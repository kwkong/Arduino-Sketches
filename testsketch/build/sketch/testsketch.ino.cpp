#include <Arduino.h>
#line 1 "c:\\Arduino\\My Sketches\\testsketch\\testsketch.ino"
#line 1 "c:\\Arduino\\My Sketches\\testsketch\\testsketch.ino"

#line 2 "c:\\Arduino\\My Sketches\\testsketch\\testsketch.ino"
void setup();
#line 7 "c:\\Arduino\\My Sketches\\testsketch\\testsketch.ino"
void loop();
#line 2 "c:\\Arduino\\My Sketches\\testsketch\\testsketch.ino"
void setup() 
{
    Serial.begin(115200);
}

void loop() 
{
    Serial.println("HEYO");
    if(Serial.available())
    {
        while(1);
    }
}

