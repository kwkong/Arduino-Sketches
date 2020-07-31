#include <SoftwareSerial.h>
SoftwareSerial BT(10, 11); 
// creates a "virtual" serial port/UART
// connect BT module TX to D10
// connect BT module RX to D11
// connect BT Vcc to 5V, GND to GND

int num;

void setup()  
{

  Serial.begin(9600);
  pinMode(13, OUTPUT);    //configure arduino LED
  BT.begin(9600);         //set the data rate for the SoftwareSerial port
}

void loop() 
{
  if (BT.available())      //if text arrived in from BT serial...
    {
      num=(BT.read());    //Store hex command in integer. Also converts hex to decimal
      Serial.println(num);
          
      if (num == 1)        //Blink LED if a '1' is sent 
        {
          digitalWrite(13, HIGH);
        }
      if (num == 2)
        {
          digitalWrite(13, LOW);
        }
       
    }   
}



