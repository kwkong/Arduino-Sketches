#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

int num;

void setup() {

Serial.begin(9600);

mySerial.begin(9600);

}

void loop()

{


if (Serial.available())
mySerial.write(Serial.read());


}
