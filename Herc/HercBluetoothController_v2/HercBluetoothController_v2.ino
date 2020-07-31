/*
Bluetooth Phone to Herc Controller
Author: Kriston Kong
v2 written 19/9/2017
*/


#include <Hercules.h>
#include <Hercules_dfs.h>
#include <Hercules_Stepper.h>
#include <seeed_pwm.h>
#include <RobotMovement4.h>
#include <Servo.h>
#include <SoftwareSerial.h>

// Button Number Hex Commands
// Dec   Hex
// Forward =     2  2;
// Left =        7  7;
// Stop =        5  5;
// Right =       6  6;
// Back =        8  8;
// Fire =      16  22;

//Hex Commands for respective functions

const int Forward = 2;
const int Left = 7;
const int Stop = 5;
const int Right = 6;
const int Back = 8;
const int Fire = 22;

const int triggerPin = 2;
const int launcherPin = 3;
const int buttonPin = 4;
//const int launcherPin = 12;

int num = 0;

int openAngle = 110; // Servo (trigger) angle to open the trigger
int closeAngle = 30; // Servo (trigger) angle to close the trigger
int launchAngle = 90; // Servo (launcher) angle to pull the spring to, when launching
int loadAngle = 45; // Servo (launcher) angle at which the launcher is in the down position

Servo launcher;
Servo trigger;
//Servo platform;


void setup()
{
    pinMode(13, OUTPUT); // configure arduino LED

    launcher.attach(launcherPin);
    trigger.attach(triggerPin);
    launcher.write(loadAngle); // default positions for launcher
    trigger.write(openAngle);
    delay(1000);

    //Startup sequence for debugging

    // digitalWrite(13, HIGH);
    // delay(1000);
    // digitalWrite(13, LOW);
    // delay(1000);
    // digitalWrite(13, HIGH);
    // delay(1000);
    // digitalWrite(13, LOW);


    Serial.begin(9600);
    Serial1.begin(9600);
    Serial2.begin(9600);
    Move.begin(52);
}

void loop()
{
    bluetoothRead();
    moveControl();
    fireControl();
}

void fire()
{
    trigger.write(openAngle); // open trigger
    launcher.write(loadAngle); // load launcher
    delay(1000);
    trigger.write(closeAngle); // close trigger
    delay(1000);
    launcher.write(launchAngle); // charge launcher
    delay(1000);
    trigger.write(openAngle); // open trigger, releasing launcher
    delay(1000);
    launcher.write(loadAngle); // reset launcher
    delay(1000);
}

void moveControl()
{
    if (num == Forward)
    {
        // go forward
        Move.Forward(40);
        delay(3);
    }
    if (num == Back)
    {
        // go back
        Move.Reverse(30);
        delay(3);
    }
    if (num == Left)
    {
        // go left
        Move.TurnLeft(45);
        delay(3);
    }
    if (num == Right)
    {
        // go right
        Move.TurnRight(45);
        delay(3);
    }
    if (num == Stop)
    {
        // stop
        Move.Stop();
    }
    else
    {
        Move.Stop();
    }
}

void fireControl()
{
    if (num == Fire)
    {
        fire();
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
