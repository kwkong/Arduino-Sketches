#include <Wire.h>

#define slave1Addr 9
#define slave2Addr 10

#define trigPin1 2
#define trigPin2 3
#define signalPin1 7
#define signalPin2 8

int val = 0;
int mode = 0;
int addr = 0;

String inputString = "";
boolean commComplete = false;     
String msgString = "";
boolean msgComplete = false;   

void setup()
{
	Serial.begin(9600); 
	Wire.begin();

	pinMode(trigPin1, INPUT);
	pinMode(trigPin2, INPUT);
	pinMode(signalPin1, OUTPUT);
	pinMode(signalPin2, OUTPUT);

	attachInterrupt(digitalPinToInterrupt(trigPin1), detect1, RISING);
	attachInterrupt(digitalPinToInterrupt(trigPin2), detect2, RISING);

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
		Serial.println("Running");
	}
}


void detect1() //INTERRUPT
{
	val = 1;
	Serial.println("Detected 1");
}

void detect2() //INTERRUPT
{
	val = 1;
	Serial.println("Detected 2");
}

void configMode()
{
	Serial.println("Enter Address");
	getSerial();

	addr = inputString.toInt();

	if ((addr == slave1Addr) || (addr == slave2Addr))
	{
		Serial.println("Enter Command");
		getSerial();

		if (inputString == nlcr("on"))
		{
			if (addr == slave1Addr)
			{
				digitalWrite(signalPin1, HIGH);
				Serial.println("Detector 1 Enabled");
			}

			if (addr == slave2Addr)
			{
				digitalWrite(signalPin2, HIGH);
				Serial.println("Detector 2 Enabled");
			}
		}

		else if (inputString == nlcr("off"))
		{
			if (addr == slave1Addr)
			{
				digitalWrite(signalPin1, LOW);
				Serial.println("Detector 1 Disabled");
			}

			if (addr == slave2Addr)
			{
				digitalWrite(signalPin2, LOW);
				Serial.println("Detector 2 Disabled");
			}
		}

		else if (inputString == nlcr("a thresh"))
		{
			Wire.beginTransmission(addr);
			Wire.write('a');
			Wire.write('\n');
			Wire.endTransmission();

			Serial.println("Enter New Threshold");

			getSerial();

			Wire.beginTransmission(addr);
			Wire.write(inputString.c_str());
			Wire.endTransmission();
		}

		else if (inputString == nlcr("q back"))
		{
			Wire.beginTransmission(addr);
			Wire.write('b');
			Wire.write('\n');
			Wire.endTransmission();

			Wire.end();

			Wire.begin(8);
			Wire.onReceive(receiveEvent); 

			while(msgComplete == false)
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

				int x = Wire.read();    // receive byte as an intege
			}
			
			Wire.end();

			Serial.println(msgString);
			msgString ="";
			msgComplete = false;

			Wire.begin();
		}

		else if (inputString == nlcr("a back"))
		{
			Wire.beginTransmission(addr);
			Wire.write('d');
			Wire.write('\n');
			Wire.endTransmission();

			Serial.println("Enter New Background Value");

			getSerial();

			Wire.beginTransmission(addr);
			Wire.write(inputString.c_str());
			Wire.endTransmission();
		}



		else if (inputString == nlcr("trig"))
		{

			if (addr == slave1Addr)
			{
				digitalWrite(signalPin1, LOW);
				delay(25);
				Wire.beginTransmission(addr);
				Wire.write('t');
				Wire.write('\n');
				Wire.endTransmission();
				delay(100);
			}

			if (addr == slave2Addr)
			{
				digitalWrite(signalPin2, LOW);
				delay(25);
				Wire.beginTransmission(addr);
				Wire.write('t');
				Wire.write('\n');
				Wire.endTransmission();
				delay(100);
			}
		}

		else if (inputString == nlcr("run"))
		{
			mode = 0;
			inputString="";
		}

		else
		{
			Serial.println("Invalid Command");
		}
	}

	else
	{
		Serial.println("Invalid Address");
		Serial.println("Return to Run? (y/n)");

		getSerial();

		if (inputString == nlcr("y"))
		{
			mode = 0;
		}

		else
		{
			
		}

	}
}

String nlcr(String command)
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
			if (inputString == nlcr("config"))
			{
				mode = 1;
			}

			else if (inputString == nlcr("run"))
			{
				mode = 0;		
			}

			inputString = "";
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