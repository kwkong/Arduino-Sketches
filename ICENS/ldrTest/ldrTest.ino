#define LDRpin A0
#define signalPin 7

#define settleSize 100
#define settleTime 50

int val;

int thresh;



void setup()
{
	Serial.begin(9600);
	settle();
}

void loop()
{
	// val = analogRead(LDRpin);
	// //Serial.println(val);
	// //delay(50);

	// if (val > 280)
	// {
	// 	Serial.println("Sample Detected");
	// }

	// else 
	// {
	// 	Serial.println("No Sample");
	// }
}

void settle()
{
	int settleBit = 0;
	int vals[settleSize];
	int valSum;
	int valAvg1, valAvg2;

	while (settleBit != 1)
	{
		valSum = 0;

		for (int i=0; i<settleSize; i++)
		{
			vals[i] = analogRead(LDRpin);
			valSum += vals[i];
			delay(settleTime);
		}
 	
		valAvg1 = valSum / settleSize;

		valSum = 0;

		for (int i=0; i<settleSize; i++)
		{
			vals[i] = analogRead(LDRpin);
			valSum += vals[i];
			delay(settleTime);
		}
 	
		valAvg2 = valSum / settleSize;

		if((valAvg1 - valAvg2) < 5)
		{
			settleBit = 1;
			thresh = ((valAvg2 + valAvg1) / 2)
			break;
		}

		else
		{
			Serial.println("Settle Failed");
		}
	}

	Serial.println("Settled");
}

void detect()
{
	while(1)
	{
		val = analogRead(LDRpin);

		if (val > thresh)
		{
			digitalWrite(signalPin, HIGH);
		}

		else
		{
			digitalWrite(signalPin, LOW);
		}
	}
}

void detectTest()
{
	int setBit;

	while(setBit != 1)
	{
		val = analogRead(LDRpin);

		if (val > 280)
		{
			Serial.println("Sample Detected");

			setBit = 1;
		}
	}
}

