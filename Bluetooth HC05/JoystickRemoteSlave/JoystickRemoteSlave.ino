#include <Servo.h>

#define chanA1 5
#define chanA2 6
#define chanB1 11
#define chanB2 3
#define servoPin 10

#define DEBUG false

Servo myServo;

int msg = 0;
int moving = 0;

void setup() 
{
    myServo.attach(servoPin);
    pinMode(13, OUTPUT);
    Serial.begin(38400);
    // openServo();

//closeServo();
   //while(1);
}

void loop()
{
    if (Serial.available())
    {
        msg=(Serial.read());
    }

    if (msg == 'R')
    {
        moveRight();
        Serial.println("R");
    }

    else if (msg == 'L')
    {
        moveLeft();
        Serial.println("L");
    }

    else if (msg == 'F')
    {
        moveForward();
        Serial.println("F");
    }

    else if (msg == 'B')
    {
        moveBack();
        Serial.println("B");
    }

    else if (msg == 'O')
    {
        openServo();
        Serial.println("O");
    }

    else if (msg == 'C')
    {
        closeServo();
        Serial.println("C");
    }

    else
    {
        moveStop();
        Serial.println("S");
    }


}

void initMotors()
{
    pinMode(chanA1, OUTPUT);
    pinMode(chanA2, OUTPUT);
    pinMode(chanB1, OUTPUT);
    pinMode(chanB2, OUTPUT);
}

void moveRight()
{
    if(DEBUG)
    {
        Serial.println("Right");
    }

    analogWrite(chanA1, 200);
    analogWrite(chanB2, 200);
    moving = 1;
}

void moveLeft()
{
    if(DEBUG)
    {
        Serial.println("Left");
    }

    analogWrite(chanA2, 200);
    analogWrite(chanB1, 200);
    moving = 1;
}

void moveForward()
{
    if(DEBUG)
    {
        Serial.println("Forward");
    }

    analogWrite(chanA2, 150);
    analogWrite(chanB2, 150);
    moving = 1;
}

void moveBack()
{
    if(DEBUG)
    {
        Serial.println("Back");
    }
    
    analogWrite(chanA1, 150);
    analogWrite(chanB1, 150);
    moving = 2;
}

void moveStop()
{
    if(DEBUG)
    {
        Serial.println("Stop");
    }

    if(moving == 1)
    {
        analogWrite(chanA2, 100);
        analogWrite(chanB2, 100);
        delay(50);
        moving = 0;
    }

    if(moving == 2)
    {
        analogWrite(chanA1, 100);
        analogWrite(chanB1, 100);
        delay(50);
        moving = 0;
    }

    analogWrite(chanA1, 0);
    analogWrite(chanA2, 0);
    analogWrite(chanB1, 0);
    analogWrite(chanB2, 0);
}

void openServo()
{
    myServo.write(170);
    //delay(200);
}

void closeServo()
{
    myServo.write(120);
    //delay(200);
}