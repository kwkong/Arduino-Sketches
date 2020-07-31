#include <Wire.h>

#define masterAddr 8
#define myAddr 9

#define LDRpin A0
#define trigPin 2
#define signalPin 7

#define settleSize 100
#define settleTime 10

#define bufferSize 20

String msgString = "";
String inputString = "";
boolean msgComplete = false;
int dir = 1;

int backgroundVal = 0;
int thresh = 10;        

void setup()
{
	Serial.begin(9600);
	Wire.begin(myAddr);                
	Wire.onReceive(receiveEvent);
	Wire.onRequest(requestEvent); 

	pinMode(trigPin, OUTPUT);
	digitalWrite(trigPin, HIGH);
	pinMode(signalPin, INPUT);

	msgString.reserve(100);
	inputString.reserve(100);

	settle();
}

void loop()
{
	// while(digitalRead(signalPin))
	// {
	// 	detect();
	// 	Serial.println("Detecting");
	// }

	getCommand();

	// Serial.println("Dir?");
	// getSerial();
	// dir = inputString.toInt();
	// Serial.println(dir);
	// digitalWrite(trigPin, LOW);
	// delay(25);
	// digitalWrite(trigPin, HIGH);

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

		switch (msgString.charAt(0)) 
		{  
		    case 'a':	//Adjust Threshold Value

				Serial.println("Enter thresh");
				msgString = "";

				while (msgComplete == false) 
				{
					receiveEvent();
				}

				Serial.println(msgString.toInt());
				thresh = msgString.toInt();
				break;

		    case 't':	//Test Interrupt Trigger

				delay(50);
				digitalWrite(trigPin, LOW);
				delay(10);
				digitalWrite(trigPin, HIGH);

				Serial.println("Signal Sent");
		     	break;

     		case 'b':	//Query Background value, see requestEvent

     			delay(50);
     			break;

 			case 'g':	//Query Background value, see requestEvent

     			delay(50);
     			break;

			case 'd':	//Adjust Background Value

				Serial.println("Enter backVal");
				msgString = "";

				while (msgComplete == false)
				{
					receiveEvent();
				}

				Serial.println(msgString.toInt());
				backgroundVal = msgString.toInt();
				break;		

		    default:

		    	break;
		}

		msgString="";
		msgComplete = false;
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

void requestEvent() 
{
	String msg = "";

	switch (msgString.charAt(0))
	{
		case 'b':

			msg = String(backgroundVal);
			Wire.write(msg.c_str());
			break;

		case 'g':

			msg = String(thresh);
			Wire.write(msg.c_str());
			break;

		default:

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