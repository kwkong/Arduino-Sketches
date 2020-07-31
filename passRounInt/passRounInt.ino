#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX

int val;
int vals[20];
int valSum;
char letter;
long num;

void setup() 
{
pinMode(13, OUTPUT);
mySerial.begin(38400);
Serial.begin(9600);
}

void loop()
{
  valSum = 0;
  
  for (int i=0; i<20; i++)
  {
    val = analogRead(A0);
    vals[i] = val;
    //val = map(val, 0, 1023, 0, 255);
    
    valSum += vals[i];
  }

  val = valSum/20;
  
  mySerial.println(val);
 // Serial.println(val);
  delay(20);

  if (mySerial.available())
  {
    num = mySerial.read();
  }

  Serial.println(num);
}





