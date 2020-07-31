#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX


int num;

void setup() {

pinMode(13, OUTPUT);
Serial.begin(9600);

mySerial.begin(9600);

}

void loop()

{

if (mySerial.available())
{
    num=(mySerial.read());    //Store hex command in integer. Also converts hex to decimal

    if (num == '1')
    {
      digitalWrite(13, HIGH);
    }

    if (num == '0')
    {
      digitalWrite(13, LOW);
    }
}
}
