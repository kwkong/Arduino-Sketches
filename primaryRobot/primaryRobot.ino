#include <Wire.h>
#include <Adafruit_Colour_Sensor.h>
#include <Hercules.h>
#include <Hercules_dfs.h>
#include <Hercules_Stepper.h>
#include <seeed_pwm.h>
#include <RobotMovement4.h>
#include <Servo.h>

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

const int redLed = 5;
const int yelLed = 6;
const int grnLed = 7;

const int triggerPin = 4;
const int launcherPin = 3;

int openAngle = 110;    //Servo (trigger) angle to open the trigger
int closeAngle = 30;    //Servo (trigger) angle to close the trigger
int launchAngle = 90;   //Servo (launcher) angle to pull the spring to, when launching
int loadAngle = 40;      //Servo (launcher) angle at which the launcher is in the down position

Servo launcher;
Servo trigger;  

int colour;


void setup()
{
    Serial.begin(9600);
    Serial1.begin(9600);

    Move.begin(52);

    colourInit();

    pinMode(redLed, OUTPUT);
    pinMode(yelLed, OUTPUT);
    pinMode(grnLed, OUTPUT);

	launcher.attach(launcherPin); 
	trigger.attach(triggerPin); 

	launcher.write(loadAngle);      //default positions for each servo
	trigger.write(openAngle);
}

void loop()
{
	getColour();
	//getBin();
	//getRawColour();
}

void getColour()
{
	uint16_t red, green, blue, clear;
    uint16_t reds[10], greens[10], blues[10];
    uint16_t redSum, greenSum, blueSum;

    for (int i=0; i<10; i++)
    {
   		tcs.getRawData(& red, & green, & blue, &clear);

   		reds[i] = red;
   		greens[i] = green;
   		blues[i] = blue;

   		redSum += reds[i];
   		greenSum += greens[i];
   		blueSum += blues[i];
	}

	red = redSum / 10;
	green = greenSum / 10;
	blue = blueSum / 10;


    if ((red > 0) && (red < 2000) && (green > 1000) && (green < 8000) && (blue > 1000) && (blue < 3000))
    {
        Serial.println("GREEN");

        digitalWrite(grnLed, HIGH);
        digitalWrite(yelLed, LOW);
        digitalWrite(redLed, LOW);

        //delay(1000);

        colour = 1;
    }

    else if ((red > 000) && (red < 1000) && (green > 000) && (green < 1000) && (blue > 1000) && (blue < 8000))
    {
        Serial.println("YELLOW");
        
        digitalWrite(grnLed, LOW);
        digitalWrite(yelLed, HIGH);
        digitalWrite(redLed, LOW);

        //delay(1000);

        colour = 2;
    }

    else if ((red > 1000) && (red < 7000) && (green > 000) && (green < 1000) && (blue > 000) && (blue < 3000))
    {
        Serial.println("RED");

       	digitalWrite(grnLed, LOW);
        digitalWrite(yelLed, LOW);
        digitalWrite(redLed, HIGH);

        //delay(1000);

        colour = 3;
    }

    else
    {
        Serial.println("I'm not sure what colour that is.");

        digitalWrite(yelLed, LOW);
        digitalWrite(grnLed, LOW);
        digitalWrite(redLed, LOW);

        colour = 0;
    }

    //delay(300);
}

void colourInit()
{
	Serial.println("Color View Test!");

    if (tcs.begin())
    {
        Serial.println("Found sensor");
    }

    else
    {
        Serial.println("No TCS34725 found ... check your connections");
        while (1);
    }
}


void fire()
{
    trigger.write(openAngle);   //open trigger
    launcher.write(loadAngle);  //load launcher
    delay(1000);

    trigger.write(closeAngle);  //close trigger
    delay(1000);

    launcher.write(launchAngle);    //charge launcher
    delay(1000);

    trigger.write(openAngle);   //open trigger
    delay(1000);

    launcher.write(loadAngle);  //reset launcher
    delay(1000);
}

void getBin()
{	
	if (colour == 1)
	{
		fire();
	}

	else if (colour == 2)
	{
		Move.TurnRight(30);
		delay (200);
		Move.Stop();

		fire();

		Move.TurnLeft(30);
		delay (200);
		Move.Stop();
	}

	else if (colour == 3)
	{
		Move.TurnLeft(30);
		delay (200);
		Move.Stop();

		fire();

		Move.TurnRight(30);
		delay (200);
		Move.Stop();
	}

	else 
	{
		Move.Stop();
	}
}

void getRawColour()
{

	uint16_t red, green, blue, clear;
	uint16_t reds[10], greens[10], blues[10];
	uint16_t redSum, greenSum, blueSum;

    for (int i=0; i<10; i++)
    {
   		tcs.getRawData(& red, & green, & blue, &clear);

   		reds[i] = red;
   		greens[i] = green;
   		blues[i] = blue;

   		redSum += reds[i];
   		greenSum += greens[i];
   		blueSum += blues[i];

	}

	red = redSum / 10;
	green = greenSum / 10;
	blue = blueSum / 10;

	//tcs.getRawData(& red, & green, & blue, &clear);

	Serial.print("Red: "); Serial.print(red);
	Serial.print("    Green: "); Serial.print(green);
	Serial.print("    Blue: "); Serial.println(blue);
}