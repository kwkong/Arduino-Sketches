
#include <Servo.h>

int upPos = 90;
int downPos = 10;
int millisDelay = 2000;



int ardyPin = 2;
int playerPin = 3;
int buzzPin = 4;
int ledPin1 = 5;
int ledPin2 = 6;
int buttonPin  = 7;

int buttonState = 1 ;
int randDelay;
int startMillis1 = 0;
int startMillis2 = 0;
int randNum;

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

	//while (((millis()) - startMillis1) >= millisDelay)

	while (buttonState != 0)
	{

		startMillis2 = millis();

		buttonState = digitalRead(buttonPin);

		if (((millis()) - startMillis2) >= randDelay)
		{
			ardy.write(downPos);
		}


		// if (buttonState != 1)
		// {
		// 	player.write(downPos);
		// 	startMillis1 = millis();
		// }
		
	}
		player.write(downPos);
		delay(100);
		ardy.write(downPos);
		delay(1000);

		ardy.write(upPos);
		player.write(upPos);
		digitalWrite(13, LOW);

		delay(1000);
}

