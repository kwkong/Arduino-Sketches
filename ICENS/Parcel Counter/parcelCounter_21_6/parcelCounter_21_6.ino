#include <Wire.h>
#include <PinChangeInt.h>

#define slave1Addr 1
#define slave2Addr 2
#define slave3Addr 3
#define slave4Addr 4

#define trigPin1 4
#define trigPin2 6
#define trigPin3 8
#define trigPin4 10

#define signalPin1 5
#define signalPin2 7
#define signalPin3 9
#define signalPin4 11

volatile int val = 0;

int dir = 0;
int mode = 0;
int addr = 0;

String inputString = "";    
String reqString = "";

unsigned long time;   

void setup()
{
	Serial.begin(9600); 
	Wire.begin();

	pinMode(signalPin1, OUTPUT);
	pinMode(signalPin2, OUTPUT);

	pinMode(trigPin1, INPUT_PULLUP);
	PCattachInterrupt(trigPin1, pcIntVect1, FALLING);
	pinMode(trigPin2, INPUT_PULLUP);
	PCattachInterrupt(trigPin2, pcIntVect2, FALLING);

	inputString.reserve(20);
	reqString.reserve(10);
}

void loop()
{
	getMode();
	sendEvent();

	if (mode == 1)
	{
		configMode();	
	}

	else
	{

	}
}


void pcIntVect1() //INTERRUPT
{
	//Serial.println("Detected 1");
	val = 1;
}

void pcIntVect2() //INTERRUPT
{
	val = 2;
	//Serial.println("Detected 2");
}

void configMode()
{
	Serial.println("Enter Address");
	getSerial();

	addr = inputString.toInt();

	if ((addr == slave1Addr) || (addr == slave2Addr))	//check if address matches any of the slaves
	{
		while(1)
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

			else if (inputString == nlcr("off"))	// turn detector off
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

			else if (inputString == nlcr("thresh1?"))	// query threshold 1 value
			{
				Wire.beginTransmission(addr);
				Wire.write('a');
				Wire.write('\n');
				Wire.endTransmission();

				sendRequest(addr);
			}

			else if (inputString == nlcr("thresh2?"))	// query threshold 2 value
			{
				Wire.beginTransmission(addr);
				Wire.write('b');
				Wire.write('\n');
				Wire.endTransmission();

				sendRequest(addr);
			}

			else if (inputString == nlcr("thresh1"))		//set threshold 1 value
			{
				Wire.beginTransmission(addr);
				Wire.write('c');
				Wire.write('\n');
				Wire.endTransmission();

				Serial.println("Enter New Threshold");

				getSerial();

				Wire.beginTransmission(addr);
				Wire.write(inputString.c_str());
				Wire.endTransmission();
			}

			else if (inputString == nlcr("thresh2"))		//set threshold 2 value
			{
				Wire.beginTransmission(addr);
				Wire.write('d');
				Wire.write('\n');
				Wire.endTransmission();

				Serial.println("Enter New Threshold");

				getSerial();

				Wire.beginTransmission(addr);
				Wire.write(inputString.c_str());
				Wire.endTransmission();
			}

			else if (inputString == nlcr("backval1?"))		//query background 1 value
			{
				Wire.beginTransmission(addr);
				Wire.write('e');
				Wire.write('\n');
				Wire.endTransmission();

				sendRequest(addr);
			}

			else if (inputString == nlcr("backval2?"))		//query background 2 value
			{
				Wire.beginTransmission(addr);
				Wire.write('f');
				Wire.write('\n');
				Wire.endTransmission();

				sendRequest(addr);
			}

			else if (inputString == nlcr("backval1"))		// set background 1 value
			{
				Wire.beginTransmission(addr);
				Wire.write('g');
				Wire.write('\n');
				Wire.endTransmission();

				Serial.println("Enter New Background Value");

				getSerial();

				Wire.beginTransmission(addr);
				Wire.write(inputString.c_str());
				Wire.endTransmission();
			}

			else if (inputString == nlcr("backval2"))		// set background 2 value
			{
				Wire.beginTransmission(addr);
				Wire.write('h');
				Wire.write('\n');
				Wire.endTransmission();

				Serial.println("Enter New Background Value");

				getSerial();

				Wire.beginTransmission(addr);
				Wire.write(inputString.c_str());
				Wire.endTransmission();
			}

			else if (inputString == nlcr("trig"))		//test interrupt trigger
			{
				int trigTest = 1;

				switch (addr) 
				{
					case slave1Addr:

						Wire.beginTransmission(addr);
						Wire.write('i');
						Wire.write('\n');
						Wire.endTransmission();
						Serial.println("Waiting for Test signal");

						time = millis();

						while(digitalRead(trigPin1))
						{
							if ((millis() - time) > 2000)
							{							
								trigTest = 0;
								break;
							}
						}

						if (trigTest == 1)
						Serial.println("Test Signal Detected");

						else
						Serial.println("Request Timed Out, Test signal not received.");

						break;

					case slave2Addr:

						Wire.beginTransmission(addr);
						Wire.write('i');
						Wire.write('\n');
						Wire.endTransmission();
						Serial.println("Waiting for Test signal");

						time = millis();
						while(digitalRead(trigPin1))
						{
							if ((millis() - time) > 2000)
							{							
								trigTest = 0;
								break;
							}
						}

						if (trigTest == 1)
						Serial.println("Test Signal Detected");

						else
						Serial.println("Request Timed Out, Test signal not received.");

						break;

					case slave3Addr:

						Wire.beginTransmission(addr);
						Wire.write('i');
						Wire.write('\n');
						Wire.endTransmission();
						Serial.println("Waiting for Test signal");

						time = millis();
						while(digitalRead(trigPin1))
						{
							if ((millis() - time) > 2000)
							{							
								trigTest = 0;
								break;
							}
						}

						if (trigTest == 1)
						Serial.println("Test Signal Detected");

						else
						Serial.println("Request Timed Out, Test signal not received.");
						break;

					case slave4Addr:

						Wire.beginTransmission(addr);
						Wire.write('i');
						Wire.write('\n');
						Wire.endTransmission();
						Serial.println("Waiting for Test signal");

						time = millis();
						while(digitalRead(trigPin1))
						{
							if ((millis() - time) > 2000)
							{							
								trigTest = 0;
								break;
							}
						}

						if (trigTest == 1)
						Serial.println("Test Signal Detected");

						else
						Serial.println("Request Timed Out, Test signal not received.");

						break;	

					default:
						break;
				}
			}

			else if (inputString == nlcr("run"))		//exits config mode
			{
				mode = 0;
				inputString="";
				break;
			}

			else if(inputString == nlcr("back"))		//go back to address select
			{
				break;
			}

			else
			{
				Serial.println("Invalid Command");
			}
		}
	}

	else
	{
		Serial.println("Invalid Address");
		Serial.println("Return to Run? (y/n)");
		getSerial();

		if (inputString == nlcr("y"))
		{
			mode = 0;	//exits config mode
		}
	}
}

String nlcr(String command)		//prepends argument string with NL and CR
{
	command +='\r'; 
	command +='\n';

	return command;
}

String getSerial()		//Waits for the user to send command via Serial
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

void getMode()		//Check for serial commands from the user
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

void sendRequest(int address)		//Sends an I2C request to get data from slave
{
	reqString= "";

	Wire.requestFrom(address, 8);    // request 6 bytes from slave device #8

	while (Wire.available()) 
	{
		char c = Wire.read();
		reqString += c;
	}

	//Serial.println(reqString);
}

void sendEvent()		//Send event information to system
{
	if (val == 1)
	{
		sendRequest(1);
		dir = reqString.toInt();

		if (dir == 1)
		{
			Serial.println("Forward");
		}

		else if (dir == 2)
		{
			Serial.println("Backward");
		}

		val = 0;
	}
}