#include <Servo.h>

Servo launcher;
Servo trigger;  
Servo platform;


const int triggerPin = 2;
const int platformPin = 3;
const int buttonPin = 4;
const int launcherPin = 12;

int val;
int platformPos = 90;

int openAngle = 110;	//Servo (trigger) angle to open the trigger
int closeAngle = 30;	//Servo (trigger) angle to close the trigger
int launchAngle = 90;	//Servo (launcher) angle to pull the spring to, when launching
int loadAngle = 0;		//Servo (launcher) angle at which the launcher is in the down position
int centerAngle = 90;	//Servo (platform) angle at which the platform aims directly forward


void setup() 
{
	pinMode(buttonPin, INPUT_PULLUP);

	launcher.attach(launcherPin); 
	trigger.attach(triggerPin); 
	platform.attach(platformPin);

	launcher.write(loadAngle);		//default positions for each servo
	trigger.write(openAngle);
	platform.write(centerAngle);

	delay(1000);

}

void loop() 
{
	stickControl();
	//buttonPress();
	fire();
	delay(2000);
}


void stickControl()
{
	val = analogRead(A0);		//reads joystick VRx potentiometer value

	//Serial.println(val);  //debugging 

	if (val <= 400)		//threshold for turning right with the joystick
	{
		platformPos += 3;

		if (platformPos > 175)
		{
			platformPos = 175;   //sets maximum angle of platform to 175
		}

		platform.write(platformPos);
		delay(50);
	}

	if (val >= 600)		//threshold for turning left
	{
		platformPos -= 3;

		if (platformPos < 5)
		{
			platformPos = 5;	//sets minimum angle of platform to 5
		}

		platform.write(platformPos);
		delay(50);
	}
}

void fire()
{
	trigger.write(openAngle);	//open trigger
	launcher.write(loadAngle);	//load launcher
	delay(1000);

	trigger.write(closeAngle);	//close trigger
	delay(1000);

	launcher.write(launchAngle);	//charge launcher
	delay(1000);

	trigger.write(openAngle);	//open trigger
	delay(1000);

	launcher.write(loadAngle);	//reset launcher
	delay(1000);
}

void buttonPress()
{
	if ((digitalRead(buttonPin)) != 1)		//read for button press
	{
		while ((digitalRead(buttonPin)) != 1);

		fire();
	}
}




