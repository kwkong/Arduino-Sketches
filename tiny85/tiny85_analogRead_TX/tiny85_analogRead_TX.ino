#include <SoftwareSerial.h>

SoftwareSerial mySerial(4, 3);

int pot;
int ldr;

void setup()
{
  mySerial.begin(9600);
}

void loop()
{
  pot = analogRead(A2);
  ldr = analogRead(A1);
//  mySerial.print("Pot Val:");
//  mySerial.print(pot);
//  mySerial.print("            LDR Val:");
//  mySerial.println(ldr);

  
  
  delay(250);
}
