#define sensorPin1 A0
#define sensorPin2 A1


#define settleSize 100
#define settleTime 10


String inputString = "";

int backgroundVal = 0;
int backgroundVal2 = 0;
int thresh1 = 5;
int thresh2 = 10;
int sensorVal1 = 0;
int sensorVal2 = 0;  

int dir = 0;
int oldDir = 0;
int trial = 0;

unsigned long time;      

void setup()
{
	Serial.begin(9600);
	//settle1();
	//settle2();
}

void loop()
{
	//waitForParcel(1);
	getParcelTime(2);		
}

void settle1(int index)		//Startup settle background value for sensor 1
{
	int settleBit = 0;
	int vals[settleSize];
	long valSum;
	int valAvg1, valAvg2;
	int valDiff;

	while (settleBit != 1)
	{
		valSum = 0;

		for (int i=0; i<settleSize; i++)
		{
			vals[i] = analogRead(sensorPin1);
			valSum += vals[i];
			delay(settleTime);
		}
 	
		valAvg1 = valSum / settleSize;

		valSum = 0;

		for (int i=0; i<settleSize; i++)
		{
			vals[i] = analogRead(sensorPin1);
			valSum += vals[i];
			delay(settleTime);
		}
 	
		valAvg2 = valSum / settleSize;
		valDiff = valAvg1 - valAvg2;

		//Serial.println(valDiff);

		if((abs(valDiff)) < 5)
		{
			settleBit = 1;
			backgroundVal1 = ((valAvg2 + valAvg1) / 2);
			break;
		}

		else
		{
			Serial.println("Settle 1 Failed");
		}
	}

	Serial.print("Settled 1 :");
	Serial.println(backgroundVal1);
}

void settle2()		//Startup settle background value for sensor 2
{
	int settleBit = 0;
	int vals[settleSize];
	long valSum;
	int valAvg1, valAvg2;
	int valDiff;

	while (settleBit != 1)
	{
		valSum = 0;

		for (int i=0; i<settleSize; i++)
		{
			vals[i] = analogRead(sensorPin2);
			valSum += vals[i];
			delay(settleTime);
		}
 	
		valAvg1 = valSum / settleSize;

		valSum = 0;

		for (int i=0; i<settleSize; i++)
		{
			vals[i] = analogRead(sensorPin2);
			valSum += vals[i];
			delay(settleTime);
		}
 	
		valAvg2 = valSum / settleSize;
		valDiff = valAvg1 - valAvg2;


		if((abs(valDiff)) < 5)
		{
			settleBit = 1;
			backgroundVal2 = ((valAvg2 + valAvg1) / 2);
			break;
		}

		else
		{
			Serial.println("Settle 2 Failed");
		}
	}

	Serial.print("Settled 2: ");
	Serial.println(backgroundVal2);
}

void getParcelSpeed()
{
	Serial.println("Detecting");

	while (1)
	{
		sensorVal1 = analogRead(sensorPin1);
		sensorVal2 = analogRead(sensorPin2);


		if (sensorVal1 > (backgroundVal1 + thresh1))
		{
			//Serial.println("Sensor 1 Triggered");
			time = micros();

			while (sensorVal1 > (backgroundVal1 + thresh1))
			{
				sensorVal1 = analogRead(sensorPin1);
			}

			time = micros() - time;
			Serial.println("Sensor 1 Triggered");
			Serial.print("Time (us): ");
			Serial.println(time);
			break;
		}

		else if (sensorVal2 > (backgroundVal2 + thresh2))
		{
			Serial.println("Sensor 2 Triggered");
			time = micros();

			while (sensorVal1 < (backgroundVal1 + thresh1))
			{
				sensorVal1 = analogRead(sensorPin1);
			}

			time = micros() - time;
			Serial.println("Sensor 1 Triggered");
			Serial.print("Time (us): ");
			Serial.println(time);
			break;
		}
	}

	settle1();
	settle2();
}

void getParcelTime(int sensorNum)
{
	

	if (sensorNum == 1)
	{

		settle1();
		Serial.println("Detecting");

		while (1)
		{
			sensorVal1 = analogRead(sensorPin1);

			if (sensorVal1 > (backgroundVal1 + thresh1))
			{
				time = micros();

				while (sensorVal1 > (backgroundVal1 + thresh1))
				{
					sensorVal1 = analogRead(sensorPin1);
				}

				time = micros() - time;

				Serial.println("Sensor 1 Triggered");
				//Serial.print("Time (us): ");
				//Serial.println(time);
				break;
			}
		}

		//settle1();
	}

	else if (sensorNum == 2)
	{
		settle2();
		Serial.println("Detecting");

		while (1)
		{
			sensorVal2 = analogRead(sensorPin2);

			if (sensorVal2 > (backgroundVal2 + thresh2))
			{
				time = micros();

				while (sensorVal2 > (backgroundVal2 + thresh2))
				{
					sensorVal2 = analogRead(sensorPin2);
				}

				time = micros() - time;

				Serial.println("Sensor 2 Triggered");
				//Serial.print("Time (us): ");
				//Serial.println(time);
				break;
			}
		}

		//settle2();
	}

	Serial.print("Time Trial ");
	Serial.print(trial);
	Serial.print("(us): ");
	Serial.print(time);
	Serial.print("    ");
	Serial.print("Threshold: ");

	if(sensorNum == 1)
	Serial.println(thresh1);

	if(sensorNum == 2)
	Serial.println(thresh2);

	trial++;
}

void waitForParcel(int sensorNum)
{
	if (sensorNum == 1)
	{
		Serial.println("Waiting for sample on sensor 1");
		while(!active(1));
		Serial.println("Sensor 1 Triggered");
	}

	else if (sensorNum == 2)
	{
		Serial.println("Waiting for sample on sensor 2");
		while(!active(2));
		Serial.println("Sensor 2 Triggered");
	}
}

String getSerial()
{
	boolean serialComplete = false;
	inputString = "";

	while (serialComplete == false)
	{
		while (Serial.available()) 
		{
			char inChar = (char)Serial.read();
			inputString += inChar;

			if (inChar == '\n') 
			{
				serialComplete = true;
			}
		}
	}

	return inputString;
}

boolean active(int sensorNum)
{
	boolean parcelEvent = false;

	if (sensorNum == 1)
	{
		sensorVal1 = analogRead(sensorPin1);

		if (sensorVal1 < (backgroundVal1 + thresh1))
		parcelEvent = false;

		else
		parcelEvent = true;

		return parcelEvent;
	}

	else if (sensorNum == 2)
	{
		sensorVal2 = analogRead(sensorPin2);

		if (sensorVal1 < (backgroundVal2 + thresh2))
		parcelEvent = false;

		else
		parcelEvent = true;

		return parcelEvent;
	}
}