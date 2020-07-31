#include <Hercules.h>
#include <Hercules_dfs.h>
#include <Hercules_Stepper.h>
#include <seeed_pwm.h>

#include <RobotMovement4.h>
#include <Servo.h>
#include <SoftwareSerial.h>


// Button Number Hex Commands
//             Dec   Hex
// Forward =     2  2;
// Left =        4  4;
// Stop =        5  5;
// Right =       6  6;
// Back =        8  8;
// AimLeft =    15  21;
// Fire =      16  22;
// AimRight =   17  23;
// Launch90 =   10  16;
// Launch60 =   14  20;
// Launch30 =   18  24;

const int Forward = 2;
const int Left = 7;
const int Stop = 5;
const int Right = 6;
const int Back = 8;
const int Fire = 22;

const int triggerPin = 2;
const int platformPin = 3;
const int buttonPin = 4;
const int launcherPin = 12;

int val;

int openAngle = 110;    //Servo (trigger) angle to open the trigger
int closeAngle = 30;    //Servo (trigger) angle to close the trigger
int launchAngle = 90;   //Servo (launcher) angle to pull the spring to, when launching
int loadAngle = 10;      //Servo (launcher) angle at which the launcher is in the down position

// const int AimLeft = 21;
// const int AimRight = 23;
// const int Launch90 = 16;
// const int Launch60 = 20;
// const int Launch30 = 24;

int num;
unsigned long t1;

Servo launcher;
Servo trigger;  
Servo platform;


// creates a "virtual" serial port/UART
// connect Serial2 module TX to D10
// connect Serial2 module RX to D11
// connect Serial2 Vcc to 5V, GND to GND

void setup()
{
    pinMode(13, OUTPUT); // configure arduino LED

    launcher.attach(launcherPin); 
    trigger.attach(triggerPin); 

    launcher.write(loadAngle);      //default positions for each servo
    trigger.write(openAngle);

    delay(1000);

    Serial.begin(9600);
    Serial1.begin(9600);
    Serial2.begin(9600);

    Move.begin(52);
    
     // set the data rate for the SoftwareSerial port
}

void loop()
{
    Serial2.flush();
    num = 0;
    if (Serial2.available())
        //if text arrived in from Serial2 serial...
    {

        num = (Serial2.read()); // Store hex command in integer. Also converts hex to decimal
        // Serial.println(num);

    }

    // -----------LAUNCH CONTROLS------------//

    // if (num == AimRight)
    // {
    //     basePos += 5;
    //     if (basePos > 170)
    //     {
    //         basePos = 170;
    //     }
    //     base.write(basePos);
    //     delay(50);
    // }

    // if (num == AimLeft)
    // {
    //     basePos -= 5;
    //     if (basePos < 0)
    //     {
    //         basePos = 0;
    //     }
    //     base.write(basePos);
    //     delay(50);
    // }

    Serial.print(num);

    if (num == Fire)
    {
        fire();
    }

    // if (num == Launch90)
    // {
    //     launchAngle = 100;
    //     arm.write(0);
    //     delay(100);
    // }

    // if (num == Launch60)
    // {
    //     launchAngle = 60;
    //     arm.write(0);
    //     delay(100);
    // }

    // if (num == Launch30)
    // {
    //     launchAngle = 30;
    //     arm.write(0);
    //     delay(100);
    // }

    // -----------MOVE CONTROLS-------------//

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