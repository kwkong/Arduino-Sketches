#include <Wire.h>

#define bufferSize 20

char msgArray[bufferSize];
String msgString = "";
int msgIndex = 0;
boolean msgComplete = false;

void setup() 
{
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
}

void loop() 
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