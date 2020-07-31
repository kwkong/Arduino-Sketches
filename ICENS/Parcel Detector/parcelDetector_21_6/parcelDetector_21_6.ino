#include <Wire.h>

#define myAddr 1

#define LDRpin1 A0
#define LDRpin2 A1
#define trigPin 4
#define signalPin 5

#define settleSize 100
#define settleTime 10

String msgString = "";
String inputString = "";
boolean msgComplete = false;
int dir = 0;

int backgroundVal1 = 0;
int backgroundVal2 = 0;
int thresh1 = 10;
int thresh2 = 10;
int sensorVal1 = 0;
int sensorVal2 = 0;  

unsigned long time;      

void setup()
{
	Serial.begin(9600);
	Wire.begin(myAddr);                
	Wire.onReceive(receiveEvent);
	Wire.onRequest(requestEvent); 

	pinMode(trigPin, OUTPUT);
	digitalWrite(trigPin, HIGH);
	pinMode(signalPin, INPUT);

	msgString.reserve(20);

	settle1();
	settle2();
}

void loop()
{
	detect();		//Breaks from detect loop when signalPin is HIGH
	getCommand();
}

void settle1()		//Startup settle background value for sensor 1
{
	int settleBit = 0;
	int vals[settleSize];
	int valSum;
	int valAvg1, valAvg2;
	int valDiff;

	while (settleBit != 1)
	{
		valSum = 0;

		for (int i=0; i<settleSize; i++)
		{
			vals[i] = analogRead(LDRpin1);
			valSum += vals[i];
			delay(settleTime);
		}
 	
		valAvg1 = valSum / settleSize;

		valSum = 0;

		for (int i=0; i<settleSize; i++)
		{
			vals[i] = analogRead(LDRpin1);
			valSum += vals[i];
			delay(settleTime);
		}
 	
		valAvg2 = valSum / settleSize;
		valDiff = valAvg1 - valAvg2;

		Serial.println(valDiff);

		if((abs(valDiff)) < 5)
		{
			settleBit = 1;
			backgroundVal1 = ((valAvg2 + valAvg1) / 2);
			break;
		}

		else
		{
			Serial.println("Settle 1 Failed");
		}
	}

	Serial.println("Settled 1");
	Serial.print(backgroundVal1);
}

void settle2()		//Startup settle background value for sensor 2
{
	int settleBit = 0;
	int vals[settleSize];
	int valSum;
	int valAvg1, valAvg2;
	int valDiff;

	while (settleBit != 1)
	{
		valSum = 0;

		for (int i=0; i<settleSize; i++)
		{
			vals[i] = analogRead(LDRpin2);
			valSum += vals[i];
			delay(settleTime);
		}
 	
		valAvg1 = valSum / settleSize;

		valSum = 0;

		for (int i=0; i<settleSize; i++)
		{
			vals[i] = analogRead(LDRpin2);
			valSum += vals[i];
			delay(settleTime);
		}
 	
		valAvg2 = valSum / settleSize;
		valDiff = valAvg1 - valAvg2;

		Serial.println(valDiff);

		if((abs(valDiff)) < 5)
		{
			settleBit = 1;
			backgroundVal2 = ((valAvg2 + valAvg1) / 2);
			break;
		}

		else
		{
			Serial.println("Settle 2 Failed");
		}
	}

	Serial.println("Settled 2");
	Serial.print(backgroundVal2);
}

void detect()
{
	while(digitalRead(signalPin))
	{
		//Serial.println("detecting");

		sensorVal1 = analogRead(LDRpin1);
		sensorVal2 = analogRead(LDRpin2);

		if (sensorVal1 > (backgroundVal1 + thresh1))
		{
			dir = 1;
			Serial.println("Sensor 1 Triggered");
		}

		else if (sensorVal2 > (backgroundVal2 + thresh2))
		{
			dir = 2;
		}

		else
		{
			dir = 0;
		}

		if (dir == 0)
		{
			digitalWrite(trigPin, HIGH);
		}

		else
		{
			digitalWrite(trigPin, LOW);
		}
	}
}

void getMsg()
{
	if (msgComplete)
	{
		msgComplete = false;
		Serial.println(msgString);
	}
}

void getCommand()
{
	if (msgComplete)
	{
		msgComplete = false;

		switch (msgString.charAt(0)) 
		{  
     		case 'a':	//Query Threshold 1 value, see requestEvent

     			delay(50);
     			break;

     		case 'b':	//Query Threshold 2 value, see requestEvent

     			delay(50);
     			break;

		    case 'c':	//Adjust Threshold 1 Value

				Serial.println("Enter thresh");
				while (msgComplete == false)
				{
					delay(10);
				}

				Serial.println(msgString.toInt());
				thresh1 = msgString.toInt();
				break;

		    case 'd':	//Adjust Threshold 2 Value

				Serial.println("Enter thresh");
				while (msgComplete == false)
				{
					delay(10);
				}

				Serial.println(msgString.toInt());
				thresh2 = msgString.toInt();
				break;

     		case 'e':	//Query Background 1 value, see requestEvent

     			delay(50);
     			break;

 			case 'f':	//Query Background 2 value, see requestEvent

     			delay(50);
     			break;

			case 'g':	//Adjust Background 1 Value

				Serial.println("Enter backVal");

				while (msgComplete == false)
				{
					delay(10);
				}
				Serial.println(msgString.toInt());
				backgroundVal1 = msgString.toInt();
				break;

			case 'h':	//Adjust Background 2 Value

				Serial.println("Enter backVal");

				while (msgComplete == false)
				{
					delay(10);
				}
				Serial.println(msgString.toInt());
				backgroundVal2 = msgString.toInt();
				break;

		    case 'i':	//Test Interrupt Trigger

				delay(50);
				digitalWrite(trigPin, LOW);
				delay(10);
				digitalWrite(trigPin, HIGH);

				Serial.println("Signal Sent");
		     	break;			

		    default:

		    	break;
		}

		msgComplete = false;
	}
}

void receiveEvent() 
{
	msgString = "";

	while (Wire.available()) 
	{
		char c = Wire.read(); // receive byte as a character
		msgString += c;	//Add byte 

		if(c == '\n')
		{
			msgComplete = true;
		}
	}
}

void requestEvent() 
{
	String msg = "";

	switch (msgString.charAt(0))
	{
		case 'a':		//Query threshold 1 value

			msg = String(thresh1);
			Wire.write(msg.c_str());
			break;

		case 'b':		//Query threshold 2 value

			msg = String(thresh2);
			Wire.write(msg.c_str());
			break;

		case 'e':		//Query backval 1

			msg = String(backgroundVal1);
			Wire.write(msg.c_str());
			break;

		case 'f':		//Query backval 2

			msg = String(backgroundVal2);
			Wire.write(msg.c_str());
			break;

		default:		//Send direction of parcel event

			msg = String(dir);
			Wire.write(msg.c_str());
			break;
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