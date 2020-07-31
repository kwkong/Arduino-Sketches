
#include <Servo.h>

int upPos = 90;
int downPos = 0;

unsigned long millisDelay = 2000;



int ardyPin = 2;
int playerPin = 3;
//int buzzPin = 4;

int redLed = 8;
int yelLed = 9;
int grnLed = 10;

int buttonPin  = 4;
int modePin = 5;
int startPin = 6;


int mode = 1;



unsigned long startMillis1;
unsigned long startMillis2;
unsigned long startMillis3;

int modeState;
int startState;
int buttonState;

int randNum;
int randDelay;

Servo player;
Servo ardy;

void setup()
{
	player.attach(playerPin);
	ardy.attach(ardyPin);

	pinMode(redLed, OUTPUT);
	pinMode(yelLed, OUTPUT);
	pinMode(grnLed, OUTPUT);
	pinMode(13, OUTPUT);


	pinMode(modePin, INPUT_PULLUP);
	pinMode(startPin, INPUT_PULLUP);
	pinMode(buttonPin, INPUT_PULLUP);

	player.write(upPos);
	ardy.write(upPos);
	delay(1000);
}

void loop()
{

	startState = 1;
	modeState = 1;

	//MODE SELECT

	while(startState != 0)
	{
		startState = digitalRead(startPin);
	    while(digitalRead(startPin) == 0);     
		modeState = digitalRead(modePin);
		while(digitalRead(modePin) == 0)    
		{
			delay(50);
		}


		if (modeState != 1)
		{
			while (modeState!=1)
			{
				modeState = digitalRead(modePin);
			}

			mode++;

			if (mode > 3)
			{
				mode = 1;
			}
		}

		switch (mode)
		{
			case 1: 
					digitalWrite(grnLed, HIGH);
					digitalWrite(yelLed, LOW);
					digitalWrite(redLed, LOW);
					
					randDelay = random(500, 1000);
					break;
			case 2:
					digitalWrite(grnLed, LOW);
					digitalWrite(yelLed, HIGH);
					digitalWrite(redLed, LOW);
					
					randDelay = random(400, 500);
					break;
			case 3:
					digitalWrite(grnLed, LOW);
					digitalWrite(yelLed, LOW);
					digitalWrite(redLed, HIGH);
					
					randDelay = random(300, 400);
					break;

			default:
					break;		
		}
	}

	//WAIT LOOP

	randNum = random(5000, 15000);

	startMillis3 = millis();

	while(((millis()) - startMillis3) <= randNum)
	{
		switch (mode)
		{
			case 1: 
					digitalWrite(grnLed, !digitalRead(grnLed));
					break;
			case 2:
					digitalWrite(yelLed, !digitalRead(yelLed));
					break;
			case 3:
					digitalWrite(redLed, !digitalRead(redLed));
					break;

			default:
					break;		
		}

		delay(50);
	}

	//START

	digitalWrite(grnLed, HIGH);
	digitalWrite(yelLed, HIGH);
	digitalWrite(redLed, HIGH);

	startMillis2 = millis();

	

	//Reaction

	buttonState = 1;

	while (buttonState != 0)
	{

		buttonState = digitalRead(buttonPin);
		while(digitalRead(buttonPin) == 0); 


		if (((millis()) - startMillis2) >= randDelay)
		{
			ardy.write(downPos);
			delay(50);
		}	
	}

		//Reset

		startState = 0;
		modeState = 0;

		player.write(downPos);
		delay(300);
		ardy.write(downPos);
		delay(1000);

		ardy.write(upPos);
		player.write(upPos);

		digitalWrite(grnLed, LOW);
		digitalWrite(yelLed, LOW);
		digitalWrite(redLed, LOW);

		delay(1000);
}

