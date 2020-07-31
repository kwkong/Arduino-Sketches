#define LDRpin A0
#define signalPin 2
#define ackPin 7

#define settleSize 100
#define settleTime 10

int val;

int backgroundVal;
int thresh = 10;

String inputString = "";         
boolean stringComplete = false; 



void setup()
{
	Serial.begin(9600);
	pinMode(signalPin, OUTPUT);
	pinMode(ackPin, INPUT);

	inputString.reserve(200);

	settle();

	
}

void loop()
{
	detect();
	commandEvent();
	getCommand();

	// val = analogRead(LDRpin);
	// Serial.println(val);
	// delay(50);
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
			backgroundVal = ((valAvg2 + valAvg1) / 2);
			break;
		}

		else
		{
			Serial.println("Settle Failed");
		}
	}

	Serial.println("Settled");
	Serial.print(backgroundVal);
}

void detectTest()
{
	int setBit = 0;

	while(setBit != 1)
	{
		val = analogRead(LDRpin);

		if (val > (backgroundVal + thresh))
		{
			Serial.println("Sample Detected");
			setBit = 1;
		}


	}
}

void detect()
{
	while(digitalRead(ackPin))
	{
		val = analogRead(LDRpin);

		if (val > (backgroundVal + thresh))
		{
			digitalWrite(signalPin, HIGH);
		}

		else
		{
			digitalWrite(signalPin, LOW);
		}
	}

	Serial.println("Detect OFF");
}

void commandEvent()
{
	while (Serial.available()) 
	{
		char inChar = (char)Serial.read();
		inputString += inChar;

		if (inChar == '\n') 
		{
			stringComplete = true;
		}
	}
}

void getCommand()
{
	if (stringComplete) 
	{
		//Serial.println(inputString);
		stringComplete = false;

		if (inputString.charAt(0) == 'k')
		{
			Serial.println("Enter Number");
			inputString = "";

			while (stringComplete == false) 
			{
				int inChar = Serial.read();

				if (isDigit(inChar)) 
				{
				  inputString += (char)inChar;
				}

				if (inChar == '\n') 
				{
					stringComplete = true;
				}
			}

			//Serial.println(inputString.toInt());
		}


	}
}

