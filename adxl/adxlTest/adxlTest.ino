/*
ADXL335
note:vcc-->5v ,but ADXL335 Vs is 3.3V
The circuit:
      5V: VCC
analog 1: x-axis
analog 2: y-axis
analog 3: z-axis
*/

//0,0 is      x365 y360 z441
//0, 90 is    x287 y356 z369
//0, 180 is   x360 y357 z296
//0, 270 is   x433 y362 z365

const int xPin = A0;                  // x-axis of the accelerometer
const int yPin = A1;                  // y-axis
const int zPin = A2;                  // z-axis (only on 3-axis models)


int x;
int xVals[30];
int xValSum;
int y;
int z;

int xAngle;
int xAngleOld;
int xAngleDif;

void setup()

{
 // initialize the serial communications:
 Serial.begin(9600);
}

void loop()
{
  xValSum = 0;
 
 for (int i=0; i<20; i++)
  {
    int x = analogRead(xPin);  //read from xPin
    int y = analogRead(yPin);  //read from yPin
    int z = analogRead(zPin);  //read from zPin
     
    x = map(x, 285, 435, 0, 180);
    x = constrain(x, 0, 180);

    xVals[i] = x;
    xValSum += xVals[i];
    
    z = map(z, 285, 435, 0, 180);
    z = constrain(z, 0, 180);

  }

  x = xValSum/30;
  
  if (z < 90)
  {
     xAngle = 180 + (180 - x);
  
    if (x > 359)
    {
      xAngle = 0;
    }
  }
  
  else 
  {
    xAngle = x;
  }
  
  Serial.print(xAngle);  //print x value on serial monitor
  //Serial.print("\t");
  //Serial.print(y);  //print y value on serial monitor
  //Serial.print("\t");
  //Serial.print(z);  //print z value on serial monitor
  Serial.print("\n");
  delay(20);

}
