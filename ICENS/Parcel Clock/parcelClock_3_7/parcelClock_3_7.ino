#define settleSize 50
#define settleTime 10
#define settleDiff 1



String inputString = "";

int sensorPin[9] = {A0, A1, A0, A0, A0, A0, A0, A0};
int backgroundVal[9];
int thresh[9];
int sensorVal[9];

int trial = 0;

unsigned long time;      

void setup()
{
	Serial.begin(9600);
}

void loop()
{
	//waitForParcel(20);
	getParcelTime(2,10);		
}

void settle(int sensorNum)		//Startup settle background value for sensor 1
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
			vals[i] = analogRead(sensorPin[sensorNum]);
			valSum += vals[i];
			delay(settleTime);
		}
 	
		valAvg1 = valSum / settleSize;

		valSum = 0;

		for (int i=0; i<settleSize; i++)
		{
			vals[i] = analogRead(sensorPin[sensorNum]);
			valSum += vals[i];
			delay(settleTime);
		}
 	
		valAvg2 = valSum / settleSize;
		valDiff = valAvg1 - valAvg2;

		if((abs(valDiff)) < settleDiff)
		{
			settleBit = 1;
			backgroundVal[sensorNum] = ((valAvg2 + valAvg1) / 2);
			break;
		}

		else
		{
			Serial.print("Settle ");
			Serial.print(sensorNum);
			Serial.println(" Failed");
		}
	}

	Serial.print("Settled ");
	Serial.print(sensorNum);
	Serial.print(": ");
	Serial.println(backgroundVal[sensorNum]);
}

void getParcelTime(int sensorNum, int threshVal)
{
	settle(sensorNum);
	thresh[sensorNum] = threshVal;

	Serial.println("Detecting");

	while (1)
	{
		sensorVal[sensorNum] = analogRead(sensorPin[sensorNum]);

		if (sensorVal[sensorNum] > (backgroundVal[sensorNum] + thresh[sensorNum]))
		{
			time = micros();

			while (sensorVal[sensorNum] > (backgroundVal[sensorNum] + thresh[sensorNum]))
			{
				sensorVal[sensorNum] = analogRead(sensorPin[sensorNum]);
			}

			time = micros() - time;

			Serial.print("Sensor ");
			Serial.print(sensorNum);
			Serial.println(" Triggered");
			break;
		}
	}

	Serial.print("Time Trial ");
	Serial.print(trial);
	Serial.print("(us): ");
	Serial.print(time);
	Serial.print("    ");
	Serial.print("Threshold: ");
	Serial.println(thresh[sensorNum]);

	trial++;
}

void waitForParcel(int threshVal)
{
	boolean event = false;

	for (int i = 0; i<9; i++)
	{
		settle(i);
	}

	Serial.println("Waiting on sample");

	while(event == false)
	{
		for (int i = 0; i<9; i++)
		{
			if(active(i, threshVal))
			{
				Serial.print("Sensor ");
				Serial.print(i);
				Serial.println(" Triggered");
				event = true;
				break;
			}
		}
	}
}

boolean active(int sensorNum, int threshVal)
{
	boolean parcelEvent = false;

	thresh[sensorNum] = threshVal;

	sensorVal[sensorNum] = analogRead(sensorPin[sensorNum]);

	if (sensorVal[sensorNum] < (backgroundVal[sensorNum] + thresh[sensorNum]))
	parcelEvent = false;

	else
	parcelEvent = true;

	return parcelEvent;
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