
#include <Servo.h>

int upPos = 90;
int downPos = 10;
int millisDelay = 2000;



int ardyPin = 2;
int playerPin = 3;
//int buzzPin = 4;

int ledPin1 = 8;
int ledPin2 = 9;
int ledPin3 = 10;
int buttonPin  = 6;
int modePin = 5;
int startPin = 4;

int modeState;


int startMillis1;
int startMillis2;
int randNum;
int randDelay;
Servo player;
Servo ardy;

void setup()
{
	player.attach(playerPin);
	ardy.attach(ardyPin);

	pinMode(buzzPin, OUTPUT);
	pinMode(ledPin1, OUTPUT);
	pinMode(ledPin2, OUTPUT);
	pinMode(13, OUTPUT);

	pinMode(buttonPin, INPUT_PULLUP);

	player.write(upPos);
	ardy.write(upPos);
	delay(1000);
}

void loop()
{

	randNum = random(5000, 15000);
	randDelay = random(10, 65);

	delay(randNum);

	//Event

	digitalWrite(13, HIGH);

	buttonState = 1;

	//Wait Loop

	while (buttonState != 0)
	{

		startMillis2 = millis();

		buttonState = digitalRead(buttonPin);

		if (((millis()) - startMillis2) >= randDelay)
		{
			ardy.write(downPos);
		}	
	}

		//Reset

		player.write(downPos);
		delay(100);
		ardy.write(downPos);
		delay(1000);

		ardy.write(upPos);
		player.write(upPos);
		digitalWrite(13, LOW);

		delay(1000);
}

