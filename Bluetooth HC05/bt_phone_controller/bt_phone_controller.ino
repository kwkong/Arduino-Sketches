/*
Bluetooth Phone to Herc Controller
Author: Kriston Kong
v2 written 19/9/2017
Edited for Basic Robot Chassis 25/5/2018
*/

#include <Servo.h>

//Pins

#define L2 3
#define L1 4
#define R1 5
#define R2 6

#define grabberPin 3

//Hex Commands for respective functions

#define Forward 2
#define Left 4
#define Stop 5
#define Right 6
#define Back 8
#define Grab 10
#define Release 14



int num = 0;

int openAngle = 110; 
int closeAngle = 30; 

Servo grabber;


void setup()
{
    initMotors();

    grabber.attach(grabberPin);
    grabber.write(openAngle); // default positions for grabber
    delay(1000);

    Serial.begin(9600);
    Serial2.begin(9600);
}

void loop()
{
    bluetoothRead();
    moveControl();
    grabControl();
}

void moveControl()
{
    if (num == Forward)
    {
        // go forward
        moveForward(150);
        delay(3);
    }

    if (num == Back)
    {
        // go back
       	moveReverse(150);
        delay(3);
    }

    if (num == Left)
    {
        // go left
        moveLeft(200);
        delay(3);
    }

    if (num == Right)
    {
        // go right
       	moveRight(200);
        delay(3);
    }

    if (num == Stop)
    {
        // stop
        moveStop();
    }

    else
    {
        moveStop();
    }
}

void grabControl()
{
    if (num == Grab)
    {
        grabber.write(closeAngle);
        delay(3);
    }

    if (num == Release)
    {
        grabber.write(openAngle);
        delay(3);
    }

}

void bluetoothRead()
{
    //serial2Flush();// ensure serial buffer is clear

    num = 0;
    if (Serial2.available())
    {
        num = (Serial2.read()); // Store hex command in integer. Also converts hex to decimal

        // Serial.println(num);     //Print num to serial monitor for debugging
        // delay(100);
    }
}

void serial2Flush()
{
     while (Serial2.available())
    {
        Serial2.read();
    }
}

void moveForward(int speed)
{
	analogWrite(L1, 0);
	analogWrite(R1, 0);

	analogWrite(L2, speed);
	analogWrite(R2, speed);
}

void moveStop()
{
	analogWrite(L1, 0);
	analogWrite(R1, 0);

	analogWrite(L2, 0);
	analogWrite(R2, 0);
}

void moveReverse(int speed)
{
	analogWrite(L1, speed);
	analogWrite(R1, speed);

	analogWrite(L2, 0);
	analogWrite(R2, 0);
}

void moveLeft(int speed)
{
	analogWrite(L1, speed);
	analogWrite(R2, speed);
	analogWrite(L2, 0);
	analogWrite(R1, 0);
}

void moveRight(int speed)
{
	analogWrite(L2, speed);
	analogWrite(R1, speed);
	analogWrite(L1, 0);
	analogWrite(R2, 0);
}

void initMotors()
{
	pinMode(L1, OUTPUT);
	pinMode(L2, OUTPUT);
	pinMode(R1, OUTPUT);
	pinMode(R2, OUTPUT);

	analogWrite(L1, 0);
	analogWrite(R1, 0);

	analogWrite(L2, 0);
	analogWrite(R2, 0);
}
