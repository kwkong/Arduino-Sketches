#include <SoftwareSerial.h>
SoftwareSerial BT(10, 11); 
// creates a "virtual" serial port/UART
// connect BT module TX to D10
// connect BT module RX to D11
// connect BT Vcc to 5V, GND to GND

#include <Wire.h>  // Comes with Arduino IDE
// Get the LCD I2C Library here: 
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
// Move any other LCD libraries to another folder or delete them
// See Library "Docs" folder for possible commands etc.
#include <LiquidCrystal_I2C.h>
// set the LCD address to 0x27 for a 16 chars 2 line display
// A FEW use address 0x3F
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the LCD I2C address

int num;

void setup()  
{
  lcd.begin(16,2);   // initialize the lcd for 16 chars 2 lines, turn on backlight
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
      
      lcd.clear();
      lcd.print(num);
      

          
      if (num == 1)        //Blink LED if a '1' is sent 
        {
          digitalWrite(13, HIGH);
          delay(500);
          digitalWrite(13, LOW);
        }
    }   
}



