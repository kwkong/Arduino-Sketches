#define LDRpin1 A0
#define LDRpin2 A1


#define settleSize 100
#define settleTime 10


String inputString = "";

int backgroundVal1 = 0;
int backgroundVal2 = 0;
int thresh1 = 30;
int thresh2 = 30;
int sensorVal1 = 0;
int sensorVal2 = 0;  

int dir = 0;
int oldDir = 0;

unsigned long time;      

void setup()
{
	Serial.begin(9600);
	settle1();
	settle2();
}

void loop()
{
	detect();		
}

void settle1()		//Startup settle background value for sensor 1
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
			vals[i] = analogRead(LDRpin1);
			valSum += vals[i];
			delay(settleTime);
		}
 	
		valAvg1 = valSum / settleSize;

		valSum = 0;

		for (int i=0; i<settleSize; i++)
		{
			vals[i] = analogRead(LDRpin1);
			valSum += vals[i];
			delay(settleTime);
		}
 	
		valAvg2 = valSum / settleSize;
		valDiff = valAvg1 - valAvg2;

		Serial.println(valDiff);

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
			vals[i] = analogRead(LDRpin2);
			valSum += vals[i];
			delay(settleTime);
		}
 	
		valAvg1 = valSum / settleSize;

		valSum = 0;

		for (int i=0; i<settleSize; i++)
		{
			vals[i] = analogRead(LDRpin2);
			valSum += vals[i];
			delay(settleTime);
		}
 	
		valAvg2 = valSum / settleSize;
		valDiff = valAvg1 - valAvg2;

		Serial.println(valDiff);

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

void detect()
{
	Serial.println("Detecting");

	while (1)
	{
		sensorVal1 = analogRead(LDRpin1);
		sensorVal2 = analogRead(LDRpin2);


		if (sensorVal1 > (backgroundVal1 + thresh1))
		{
			time = micros();

			Serial.println("Sensor 1 Triggered");

			while (sensorVal2 < (backgroundVal2 + thresh2))
			{
				sensorVal2 = analogRead(LDRpin2);
			}

			Serial.println("Sensor 2 Triggered");

			time = micros() - time;

			Serial.print("Time (us): ");
			Serial.println(time);
			break;
		}

		else if (sensorVal2 > (backgroundVal2 + thresh2))
		{
			time = micros();

			Serial.println("Sensor 2 Triggered");

			while (sensorVal1 < (backgroundVal1 + thresh1))
			{
				sensorVal1 = analogRead(LDRpin1);
			}

			Serial.println("Sensor 1 Triggered");

			time = micros() - time;

			Serial.print("Time (us): ");
			Serial.println(time);
			break;
		}
	}

	delay(1000);
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