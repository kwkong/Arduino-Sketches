#include <Wire.h>

#define signalPin 2
#define ackPin 7

int val = 0;
int mode = 0;

String inputString = "";
boolean commComplete = false;        

void setup()
{
	Serial.begin(9600); 

	pinMode(signalPin, INPUT);
	pinMode(ackPin, OUTPUT);

	attachInterrupt(digitalPinToInterrupt(signalPin), detect, RISING);

	inputString.reserve(200);
}

void loop()
{
	getCommand();

	if (mode == 1)
	{
		configMode();	
	}

	else
	{
		Serial.println("Idle");
	}
}


void detect() //INTERRUPT
{
	val = 1;
	Serial.println("Sample Detected");
}

void configMode()
{
	Wire.begin();

	int addr = 0;

	Serial.println("Enter Command");

	getSerial();

	if (inputString == command("on"))
	{
		digitalWrite(ackPin, HIGH);
		Serial.println("Detector Enabled");
	}

	else if (inputString == command("off"))
	{
		digitalWrite(ackPin, LOW);
		Serial.println("Detector Disabled");
	}

	else if (inputString == command("adjust"))
	{
		digitalWrite(ackPin, LOW);
		Serial.println("Enter ADDRESS");

		getSerial();

		addr = inputString.toInt();
		Serial.println(addr);

		Wire.beginTransmission(addr);
		Wire.write('k');
		Wire.write('\n');
		Wire.endTransmission();

		Serial.println("Enter Number");

		getSerial();

		Wire.beginTransmission(addr);
		Wire.write(inputString.c_str());
		Wire.endTransmission();
	}

	else if (inputString == command("run"))
	{
		mode = 0;
		inputString="";
	}

	else
	{
		Serial.println("Invalid Command");
	}

	Wire.end();
}

String command(String command)
{
	command +='\r'; 
	command +='\n';

	return command;
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

void getCommand()
{
	while (Serial.available()) 
	{
		char inChar = (char)Serial.read();
		inputString += inChar;

		if (inChar == '\n') 
		{
			if (inputString == command("config"))
			{
				mode = 1;
			}

			else if (inputString == command("run"))
			{
				mode = 0;		
			}

			inputString = "";
		}
	}
}




