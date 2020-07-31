#define settleSize 50
#define settleTime 10
#define settleDiff 1
#define threshDefault 10

volatile int val = 0;

int trial = 0;
int mode = 0;
int sensorPin[9] = {A0, A1, A0, A0, A0, A0, A0, A0};
int backVal[9];
int thresh[9];
int sensorVal[9];

String inString = "";    

unsigned long time;   


void setup()
{
	Serial.begin(115200); 
	inString.reserve(20);
	setThresholds();

	Serial.println("System Start");	
}

void loop()
{
//	getMode();
//
//	if (mode == 1)
//	{
//		configMode();	
//	}
//
//	else
//	{
//		Serial.println("Running");
//	}

  //waitForSample(100);

  getRawSensorVal(0);
  //getRawSensorVal(1);
}

void settle(int sensorNum)		//Startup settle background value for sensor 1
{
	int settleBit = 0;
	int vals[settleSize];
	long valSum;
	int valAvg1, valAvg2;
	int valDiff;

	while (settleBit != 1)
	{
		valSum = 0;

		for (int i=0; i<settleSize; i++)
		{
			vals[i] = analogRead(sensorPin[sensorNum]);
			valSum += vals[i];
			delay(settleTime);
		}
 	
		valAvg1 = valSum / settleSize;

		valSum = 0;

		for (int i=0; i<settleSize; i++)
		{
			vals[i] = analogRead(sensorPin[sensorNum]);
			valSum += vals[i];
			delay(settleTime);
		}
 	
		valAvg2 = valSum / settleSize;
		valDiff = valAvg1 - valAvg2;

		if((abs(valDiff)) < settleDiff)
		{
			settleBit = 1;
			backVal[sensorNum] = ((valAvg2 + valAvg1) / 2);
			break;
		}

		else
		{
			Serial.print("Settle ");
			Serial.print(sensorNum);
			Serial.println(" Failed");
		}
	}

	Serial.print("Settled ");
	Serial.print(sensorNum);
	Serial.print(": ");
	Serial.println(backVal[sensorNum]);
}

void setThresholds()		//sets the default thresholds for all the sensors
{
	for(int i=0; i<9; i++)
	{
		thresh[i] = threshDefault;   
	}
}

void configMode()		//mode for user to tweak and query sensor values
{
	int sensorNum = 0;

	inString = "";

	while(1)
	{
		Serial.println("Enter Command");
		get_inString();

		if (inStringMatch("run"))
		{
			mode = 0;
			Serial.println("Running");
			inString = "";
			break;
		}

		else if (inStringMatch("thresh?"))	// query threshold value
		{
			Serial.println("Query threshold of which sensor?");
			get_inString();

			if(inString.toInt() > 7)
			{
				Serial.println("Invalid Sensor Number (0 to 7 only");
				break;
			}

			Serial.print("Sensor ");
			Serial.print(inString.toInt());
			Serial.print(" Threshold: ");
			Serial.println(thresh[inString.toInt()]);
		}

		else if (inStringMatch("thresh"))		//set threshold 1 value
		{
			Serial.println("Set threshold of which sensor?");
			get_inString();


			if(inString.toInt() > 7)
			{
				Serial.println("Invalid Sensor Number (0 to 7 only)");
				break;
			}

			Serial.print("Enter new threshold for Sensor ");
			Serial.println(inString.toInt());
			sensorNum = inString.toInt();
			get_inString();

			if(inString.toInt() > 1023)
			{
				Serial.println("Invalid Value (must be between 0 and 1023)");
				break;
			}

			thresh[sensorNum] = inString.toInt();
			Serial.print("New Threshold Set ");
			Serial.println(thresh[sensorNum]);
		}

		else if (inStringMatch("backval?"))		//query background 1 value
		{
			Serial.println("Query background value of which sensor?");
			get_inString();

			if(inString.toInt() > 7)
			{
				Serial.println("Invalid Sensor Number (0 to 7 only");
				break;
			}
			
			Serial.print("Sensor ");
			Serial.print(inString.toInt());
			Serial.print(" Background Value: ");
			Serial.println(backVal[inString.toInt()]);
		}

		else if (inStringMatch("backval"))		// set background 1 value
		{
			Serial.println("Set background value of which sensor?");
			get_inString();

			if(inString.toInt() > 7)
			{
				Serial.println("Invalid Sensor Number (0 to 7 only");
				break;
			}

			Serial.print("Enter new background value for Sensor ");
			sensorNum = inString.toInt();
			Serial.println(inString.toInt());
			get_inString();

			if(inString.toInt() > 1023)
			{
				Serial.println("Invalid Value (must be between 0 and 1023)");
				break;
			}

			backVal[sensorNum] = inString.toInt();
			Serial.print("New background value Set ");
			Serial.println(backVal[sensorNum]);
		}

		else if (inStringMatch("test"))		//test interrupt trigger
		{
			//Test Event
		}

		else
		{
			Serial.println("Invalid Command");
		}
	}
}

boolean inStringMatch(String msg)		//prepends argument string with NL and CR
{
	boolean matchBool = false;

	msg +='\r'; 
	msg +='\n';

	if(inString == msg)
	matchBool = true;

	else
	matchBool = false;

	return matchBool;
}

String get_inString()		//Waits for the user to send command via Serial
{
	boolean serialComplete = false;
	String inString = "";

	while (serialComplete == false)
	{
		while (Serial.available()) 
		{
			char inChar = (char)Serial.read();
			inString += inChar;

			if (inChar == '\n') 
			{
				serialComplete = true;
			}
		}
	}

	return inString;
}

void getMode()		//Check for serial commands from the user
{
	while (Serial.available()) 
	{
		char inChar = (char)Serial.read();
		inString += inChar;

		if (inChar == '\n') 
		{
			if (inStringMatch("config"))
			{
				mode = 1;
			}

			inString = "";
		}
	}
}

void getSampleTime(int sensorNum, int threshVal)		//measure the event time for a given sensor @ a given threshold
{
	settle(sensorNum);
	thresh[sensorNum] = threshVal;

	Serial.println("Detecting");

	while (1)
	{
		sensorVal[sensorNum] = analogRead(sensorPin[sensorNum]);

		if (sensorVal[sensorNum] > (backVal[sensorNum] + thresh[sensorNum]))
		{
			time = micros();

			while (sensorVal[sensorNum] > (backVal[sensorNum] + thresh[sensorNum]))
			{
				sensorVal[sensorNum] = analogRead(sensorPin[sensorNum]);
			}

			time = micros() - time;

			Serial.print("Sensor ");
			Serial.print(sensorNum);
			Serial.println(" Triggered");
			break;
		}
	}

	Serial.print("Time Trial ");
	Serial.print(trial);
	Serial.print("(us): ");
	Serial.print(time);
	Serial.print("    ");
	Serial.print("Threshold: ");
	Serial.println(thresh[sensorNum]);

	trial++;
}

void waitForSample(int threshVal)		//wait for a Sample vial to pass any sensor
{
	boolean event = false;

	//for multiple sensors
	// for (int i = 0; i<9; i++)	
	// {
	// 	settle(i);
	// }

	settle(0);
	settle(1);

	Serial.println("Waiting on Sample");

	while(event == false)
	{
		for (int i = 0; i<9; i++)
		{
			if(active(i, threshVal))
			{
				if (i == 0)
				{
					Serial.print("Sensor ");
					Serial.print(i);
					Serial.println(" Triggered");
					event = true;
					break;
				}

				else if (i == 1)
				{
					Serial.print("Sensor ");
					Serial.print(i);
					Serial.println(" Triggered");
					event = true;
					break;
				}

				else
				{}
			}
		}
	}
}

void getRawSensorVal(int sensorNum)		//read the raw analogue value of a sensor
{
	Serial.println(analogRead(sensorPin[sensorNum]));
	delay(10);
}

void detectSample(int sensor1, int sensor2, int threshold)		//determine the direction of Sample vial moving through a tube
{
	//eventually change argument from sensorNum to the tube #

	settle(sensor1);
	settle(sensor2);

	while(1)
	{
		if(active(sensor1, threshold))
		{
			while(!active(sensor2, threshold));
			Serial.println("Forward");
			settle(sensor1);
			settle(sensor2);
		}

		if(active(sensor2, threshold))
		{
			while(!active(sensor1, threshold));
			Serial.println("Backward");
			settle(sensor1);
			settle(sensor2);
		}
	}
}

boolean active(int sensorNum, int threshVal)		//returns true if the given sensor is active
{
	boolean sampleEvent = false;

	thresh[sensorNum] = threshVal;

	sensorVal[sensorNum] = analogRead(sensorPin[sensorNum]);

	if (sensorVal[sensorNum] < (backVal[sensorNum] + thresh[sensorNum]))
	sampleEvent = false;

	else
	sampleEvent = true;

	return sampleEvent;
}
