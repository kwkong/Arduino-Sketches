//#include <SoftwareSerial.h>

//SoftwareSerial mySerial(4, 3);

int pot;
int ldr;
int state = 0;
unsigned long timer;

void setup()
{
   // mySerial.begin(9600);

    pinMode(0, OUTPUT);
    pinMode(3, OUTPUT);
}

void loop()
{
    pot = analogRead(A1);
    ldr = analogRead(A2);

    pot += 5;

    if (ldr >= pot)
    {
    	digitalWrite(3, HIGH);
      digitalWrite(0, HIGH);
      state = 1;
     delay(900000);
      
      while (ldr >= pot)
      {
        pot = analogRead(A1);
        ldr = analogRead(A2);
        ldr += 250;
      } 
    }
    else 
    {
     digitalWrite(3, LOW);
     digitalWrite(0, LOW);
     state = 0;
    } 
}

