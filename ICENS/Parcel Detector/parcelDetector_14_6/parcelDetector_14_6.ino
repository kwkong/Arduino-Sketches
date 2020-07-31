#include <Wire.h>

#define LDRpin A0
#define trigPin 3
#define signalPin 7

#define settleSize 100
#define settleTime 10

#define bufferSize 20

String msgString = "";

boolean msgComplete = false; 

int backgroundVal = 0;
int thresh = 10;        




void setup()
{
	Serial.begin(9600);
	Wire.begin(10);                
	Wire.onReceive(receiveEvent); 

	pinMode(trigPin, OUTPUT);
	pinMode(signalPin, INPUT);

	msgString.reserve(200);

	//settle();
}

void loop()
{
	while(digitalRead(signalPin))
	{
		detect();
	}

	getCommand();
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

	int val = 0;

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
	int val = analogRead(LDRpin);

	if (val > (backgroundVal + thresh))
	{
		digitalWrite(trigPin, HIGH);
	}

	else
	{
		digitalWrite(trigPin, LOW);
	}
}

void getMsg()
{
	if (msgComplete)
	{
		msgComplete = false;
		Serial.println(msgString);
		msgString = "";
	}
}

void getCommand()
{
	if (msgComplete) 
	{
		msgComplete = false;

		if (msgString.charAt(0) == 'a')
		{
			Serial.println("Enter Number");
			msgString = "";

			while (msgComplete == false) 
			{
				receiveEvent();
			}

			Serial.println(msgString.toInt());
			msgComplete = false;
		}

		else if (msgString.charAt(0) == 't')
		{
			//delay(50);
			digitalWrite(trigPin, HIGH);
			delay(10);
			digitalWrite(trigPin, LOW);

			Serial.println("Signal Sent");

			msgString = "";
		}
	}
}

void receiveEvent() 
{
	while (Wire.available()) 
	{
		char c = Wire.read(); // receive byte as a character
		msgString += c;	//Add byte 

		if(c == '\n')
		{
			msgComplete = true;
		}
	}

	int x = Wire.read();    // receive byte as an integer
}

