
#include <Servo.h>



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
const int Left = 21;
//const int Stop = 5;
const int Right = 23;
 const int Back = 8;
 const int BackLeft = 1;
 const int BackRight = 4;
// const int AimRight = 23;
// const int Launch90 = 16;
// const int Launch60 = 20;
// const int Launch30 = 24;

int num;
int servoPos;
int escPos;
int launchAngle;
unsigned long t1;

Servo servo;
Servo esc;



// creates a "virtual" serial port/UART
// connect Serial2 module TX to D10
// connect Serial2 module RX to D11
// connect Serial2 Vcc to 5V, GND to GND

void setup()
{
    pinMode(13, OUTPUT); // configure arduino LED

    servo.attach(3);
    esc.attach(9);

    Serial.begin(9600);
    Serial2.begin(9600);

    
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
       //  Serial.println(num);
    }

    // -----------LAUNCH CONTROLS------------//

    // if (num == AimRight)
    // {
    //     servoPos += 5;
    //     if (servoPos > 170)
    //     {
    //         servoPos = 170;
    //     }
    //     servo.write(servoPos);
    //     delay(50);
    // }

    // if (num == AimLeft)
    // {
    //     servoPos -= 5;
    //     if (servoPos < 0)
    //     {
    //         servoPos = 0;
    //     }
    //     servo.write(servoPos);
    //     delay(50);
    // }

    // if (num == Fire)
    // {
    //     esc.write(launchAngle);
    //     delay(300);
    //     esc.write(0);
    // }

    // if (num == Launch90)
    // {
    //     launchAngle = 100;
    //     esc.write(0);
    //     delay(100);
    // }

    if (num == BackRight)
    {
        servo.write(130);
        esc.write(55);
        delay(2);
    }

    if (num == BackLeft)
    {
        servo.write(55);
        esc.write(55);
        delay(2);
    }

    // -----------MOVE CONTROLS-------------//

    if (num == Forward)
    {
        // go forward
        esc.write(85);
        delay(3);
    }

    if (num == Back)
    {
        // go back
      // Move.Reverse(30);
    	esc.write(55);

      delay(3);

    }

    if (num == Left)
    {
        // go left
      servo.write(55);
      esc.write(85);
      delay(3);

    }

    if (num == Right)
    {
        // go right
      servo.write(130);
      esc.write(85);
      delay(3);
      
    }

    // if (num == Stop)
    // {
    //     // stop
    //     Move.Stop();
    // }

    else
    {
     //   Move.Stop();

    	//num = 0;

    	servo.write(90);
    	esc.write(65);



   }
}
