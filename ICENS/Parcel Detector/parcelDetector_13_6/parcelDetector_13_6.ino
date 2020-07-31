#include <Wire.h>

#define LDRpin A0
#define signalPin 2
#define ackPin 7

#define settleSize 100
#define settleTime 10

#define bufferSize 20

char msgArray[bufferSize];
String msgString = "";
int msgIndex = 0;
boolean msgComplete = false; 

int backgroundVal = 0;
int thresh = 10;        




void setup()
{
	Serial.begin(9600);
	Wire.begin(8);                
	Wire.onReceive(receiveEvent); 

	pinMode(signalPin, OUTPUT);
	pinMode(ackPin, INPUT);

	msgString.reserve(200);

	//settle();
}

void loop()
{
	//detect();
	//getMsg();
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
	int val = 0;

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

void getMsg()
{
	if (msgComplete)
	{
		msgComplete = false;

		msgString = String(msgArray);

		for(int i=0; i < bufferSize; i++)
		msgArray[i] = (char)0;

		Serial.println(msgString);
	}
}

void getCommand()
{
	if (msgComplete) 
	{
		//Serial.println(msgString);
		msgComplete = false;
		msgString = String(msgArray);

		if (msgString.charAt(0) == 'k')
		{
			Serial.println("Enter Number");
			msgString = "";

			while (msgComplete == false) 
			{
				receiveEvent();
			}

			msgString = String(msgArray);
			Serial.println(msgString.toInt());
			msgComplete = false;
		}
	}
}

void receiveEvent() 
{
	while (Wire.available()) 
	{
		char c = Wire.read(); // receive byte as a character
		msgArray[msgIndex] = c;	//Add byte 
		msgIndex++;

		if(c == '\n')
		{
			msgComplete = true;
			msgIndex = 0;
		}

		else if (msgIndex >= bufferSize)
		{
			msgComplete = true;
			msgIndex = 0;    	
		}
	}

	int x = Wire.read();    // receive byte as an integer
}

