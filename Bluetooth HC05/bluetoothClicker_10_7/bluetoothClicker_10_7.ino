#include "SoftwareSerial.h"

#define statusLED 13

#define bluAdd1 4
#define bluAdd2 5
#define bluAdd3 6
#define bluSub1 7
#define redAdd1 8
#define redAdd2 9
#define redAdd3 10
#define redSub1 11


SoftwareSerial mySerial(3,2);


int bluScore = 0;
int redScore = 0;
char ackChar ='a';		//admiral
String msg = "";

void setup()
{
	Serial.begin(9600);
	mySerial.begin(115200);

	msg.reserve(20);

	pinMode(bluAdd1, INPUT_PULLUP);
	pinMode(bluAdd2, INPUT_PULLUP);
	pinMode(bluAdd3, INPUT_PULLUP);
	pinMode(bluSub1, INPUT_PULLUP);
	pinMode(redAdd1, INPUT_PULLUP);
	pinMode(redAdd2, INPUT_PULLUP);
	pinMode(redAdd3, INPUT_PULLUP);
	pinMode(redSub1, INPUT_PULLUP);

	pinMode(statusLED, OUTPUT);	
}

void loop()
{
	msg = "";

	if (bluScore < 10)
	{
		msg += '0';
	}

	msg += String(bluScore);

	msg += "*";

	if (redScore < 10)
	{
		msg += '0';
	}

	msg += String(redScore);

	Serial.println("Waiting for ack");

	ack();

	Serial.println("Wacked");

	mySerial.println(msg);

	ack();


	while
	(
		digitalRead(bluAdd1) && 
		digitalRead(bluAdd2) &&
		digitalRead(bluAdd3) &&
		digitalRead(bluSub1) &&
		digitalRead(redAdd1) && 
		digitalRead(redAdd2) &&
		digitalRead(redAdd3) &&
		digitalRead(redSub1) 
	);

	delay(10);

	if(!digitalRead(bluAdd1))
	{
		bluScore += 1;
	}

	if(!digitalRead(bluAdd2))
	{
		bluScore += 2;
	}

	if(!digitalRead(bluAdd3))
	{
		bluScore += 3;
	}

	if(!digitalRead(bluSub1))
	{
		bluScore -= 1;
	}

	if(!digitalRead(redAdd1))
	{
		redScore += 1;
	}

	if(!digitalRead(redAdd2))
	{
		redScore += 2;
	}

	if(!digitalRead(redAdd3))
	{
		redScore += 3;
	}

	if(!digitalRead(redSub1))
	{
		redScore -= 1;
	}

	while
	(
		!(
			digitalRead(bluAdd1) && 
			digitalRead(bluAdd2) &&
			digitalRead(bluAdd3) &&
			digitalRead(bluSub1) &&
			digitalRead(redAdd1) && 
			digitalRead(redAdd2) &&
			digitalRead(redAdd3) &&
			digitalRead(redSub1)
		)
	);


	//delay(10);
}

void ack()
{
	while(ackChar != 'H')
	{
		ackChar = mySerial.read();

		if (ackChar == 'E')
		{
			mySerial.println(msg);
		}

		digitalWrite(statusLED, HIGH);
	}

	//delay(100);
	digitalWrite(statusLED, LOW);
	//delay(100);	

	ackChar = 'a';
}

