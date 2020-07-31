#include <Arduino.h>
#line 1 "c:\\Arduino\\My Sketches\\digitalWrite\\digitalWrite.ino"
#line 1 "c:\\Arduino\\My Sketches\\digitalWrite\\digitalWrite.ino"


#define blinkPin 16
#define blinkDelay 1000


#line 7 "c:\\Arduino\\My Sketches\\digitalWrite\\digitalWrite.ino"
void setup();
#line 13 "c:\\Arduino\\My Sketches\\digitalWrite\\digitalWrite.ino"
void loop();
#line 7 "c:\\Arduino\\My Sketches\\digitalWrite\\digitalWrite.ino"
void setup()
{
	pinMode(blinkPin, OUTPUT);
	Serial.begin(9600);	
}

void loop()
{
	digitalWrite(blinkPin, HIGH);
	Serial.println("Blink");
	delay(blinkDelay);
	digitalWrite(blinkPin, LOW);
	delay(blinkDelay);
}


