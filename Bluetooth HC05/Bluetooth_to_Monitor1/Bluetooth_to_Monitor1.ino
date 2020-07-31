//#include <SoftwareSerial.h>

//SoftwareSerial mySerial (10,11);

int num;
void setup()
{
    Serial.begin(9600);
    Serial2.begin(38400);
   // pinMode(13, OUTPUT); // configure arduino LED
 //   mySerial.begin(38400); // set the data rate for the SoftwareSerial port

}

void loop()
{
  //  if (mySerial.available())
  if (Serial2.available())
        //if text arrived in from BT serial...
    {
        //num = (mySerial.read()); // Store hex command in integer. Also converts hex to decimal
        num = (Serial2.read());
        Serial.println(num);
    }
}
