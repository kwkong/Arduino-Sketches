
#include <Wire.h>

String msgString = "";         
boolean msgComplete = false;

void setup() 
{
	Serial.begin(9600);
	Wire.begin(); 
	msgString.reserve(200);
}

void loop() 
{
	while (Serial.available()) 
	{
		char inChar = (char)Serial.read();
		msgString += inChar;

		if (inChar == '\n') 
		{
			msgComplete = true;
		}
	}


	if (msgComplete == true)
	{
		msgComplete = false;

		Serial.println(msgString);
	
		Wire.beginTransmission(8); // transmit to device #8

		Wire.write(msgString.c_str());

		Wire.endTransmission();    // stop transmitting

		msgString = "";
	}
}