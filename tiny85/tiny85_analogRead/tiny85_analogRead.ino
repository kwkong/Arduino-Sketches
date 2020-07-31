#include <SoftwareSerial.h>

SoftwareSerial mySerial(4, 3); //RX, TX

int val;
//int state;

//unsigned long timer;


void setup()
{
  mySerial.begin(9600);
  //pinMode(13,HIGH);
}

void loop()
{
  //val = analogRead(A0);
  mySerial.println(5);
  delay(250);
}
