#include <LiquidCrystal.h>
#include <Wire.h>

#define   CONTRAST_PIN   9
#define   BACKLIGHT_PIN  7
#define   CONTRAST 110

#define RS 12
#define EN 11
#define D4 5
#define D5 4
#define D6 3
#define D7 2




LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);


void setup()
{
  Serial.begin ( 57600 );

  //Use PWM to control contrast

  pinMode(CONTRAST_PIN, OUTPUT);
  analogWrite ( CONTRAST_PIN, CONTRAST );
    
  lcd.begin(16,2);               // initialize the lcd 

  lcd.setCursor(0, 0);
  lcd.print("Hello, World ");  
  lcd.setCursor (0, 1);       
  lcd.print ("abcdefghijklmnop");    
  delay(1000);  
}

void loop()
{
  // Do a little animation by writing to the same location
  lcd.setCursor ( 14, 1 );
  lcd.print (char(2));
  delay (200);
  lcd.setCursor ( 14, 1 );
  lcd.print ( char(0));
  delay (200);
}