#include <Wire.h>

#define signalPin 2
#define ackPin 7

String msgString = "";         
boolean msgComplete = false;

int addr = 0;
int val = 0;

String inputString = "";         
boolean stringComplete = false; 


void setup()
{
	Serial.begin(9600);

	Wire.begin(); 
	//msgString.reserve(200);

	pinMode(signalPin, INPUT);
	pinMode(ackPin, OUTPUT);

	attachInterrupt(digitalPinToInterrupt(signalPin), detect, RISING);

	inputString.reserve(200);

	//detectTest();
}

void loop()
{
	//sendCommand();

	//while(!getSerial())
	Serial.println(getSerial());
}


void detect()
{
	val = 1;
	Serial.println("Sample Detected");
}

void sendCommand()
{
	if (stringComplete) 
	{
		//Serial.println(inputString);
		
		stringComplete = false;

		if (inputString.charAt(0) == '1')
		{
			digitalWrite(ackPin, HIGH);
			Serial.println("Detector Enabled");
		}

		if (inputString.charAt(0) == '0')
		{
			digitalWrite(ackPin, LOW);
			Serial.println("Detector Disabled");
		}

		if (inputString.charAt(0) == 'k')
		{
			digitalWrite(ackPin, LOW);
			Serial.println("Enter ADDRESS");
			inputString = "";

			while(stringComplete == false)
			{
				serialEvent();

				if (stringComplete)
				{
					addr = inputString.toInt();
					Serial.println(addr);
					inputString = "";
				}
			}

			stringComplete = false;

			Wire.beginTransmission(addr); // transmit to device #8
			Wire.write('k');
			Wire.write('\n');
			Wire.endTransmission();

			Serial.println("Enter Number");

			while(stringComplete == false)
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

			Wire.beginTransmission(addr); // transmit to device #8
			Wire.write(inputString.c_str());
			Wire.endTransmission();
		}

		inputString = "";	
	}
}

void serialEvent() 
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



// val = response.indexOf('*');

// if ((response.charAt(val) == '*') && (response.charAt(val+1) == 'o') && (response.charAt(val+2) == 'n'))

