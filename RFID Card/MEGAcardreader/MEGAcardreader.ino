#include <RFID.h>

/*
PINOUT:
RC522 MODULE    Uno/Nano     MEGA
SDA             D10          D9
SCK             D13          D52
MOSI            D11          D51
MISO            D12          D50
IRQ             N/A          N/A
GND             GND          GND
RST             D9           D8
3.3V            3.3V         3.3V
*/
/* Include the standard Arduino SPI library */
#include <SPI.h>
/* Include the RFID library */
#include <RFID.h> 

/* Define the DIO used for the SDA (SS) and RST (reset) pins. */
#define SDA_DIO 9
#define RESET_DIO 8
/* Create an instance of the RFID library */
RFID RC522(SDA_DIO, RESET_DIO); 


// include the library code:
#include <LiquidCrystal.h>

#include "LedControl.h" 
LedControl lc=LedControl(36,38,40,1);

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(32, 30, 28, 26, 24, 22);


void setup()

{
lc.shutdown(0,false);
lc.setIntensity(0,8);

  Serial.begin(9600);
  /* Enable the SPI interface */
  SPI.begin(); 
  /* Initialise the RFID reader */
  RC522.init();

 
}

void loop()
{
  /* Has a card been detected? */
  if (RC522.isCard())
  {
    /* If so then get its serial number */
   
    RC522.readCardSerial();
    Serial.println("Card detected:");
    for(int i=0;i<5;i++)
    {
    Serial.print(RC522.serNum[i],DEC);
    //Serial.print(RC522.serNum[i],HEX); //to print card detail in Hexa Decimal format
    }
    Serial.println();
    Serial.println();
 

  lcd.begin(16, 2);
  lcd.print("Good Evening,");
  lcd.setCursor(0, 1);
  lcd.print("Joanne!");
  delay(2000);
  lcd.begin(16, 2);
  lcd.print("How was work?");
  delay(5000);
  lcd.begin(16, 2);
  lcd.print("That's nice dear.");
  
  lc.setLed(0,3,7,true);
  lc.setLed(0,2,6,true);
  lc.setLed(0,1,5,true);
  lc.setLed(0,0,4,true);
  lc.setLed(0,0,3,true);
  lc.setLed(0,1,2,true);
  lc.setLed(0,2,2,true);
  lc.setLed(0,3,3,true);
  lc.setLed(0,4,3,true);
  lc.setLed(0,5,2,true);
  lc.setLed(0,6,2,true);
  lc.setLed(0,7,3,true);
  lc.setLed(0,7,4,true);
  lc.setLed(0,6,5,true);
  lc.setLed(0,5,6,true);
  lc.setLed(0,4,7,true);
  }

  
  delay(10000);
  
}
