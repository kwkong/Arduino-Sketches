#include <Arduino.h>
#line 1 "c:\\Arduino\\My Sketches\\ledFader\\ledFader.ino"
#line 1 "c:\\Arduino\\My Sketches\\ledFader\\ledFader.ino"



#line 4 "c:\\Arduino\\My Sketches\\ledFader\\ledFader.ino"
void setup();
#line 10 "c:\\Arduino\\My Sketches\\ledFader\\ledFader.ino"
void loop();
#line 4 "c:\\Arduino\\My Sketches\\ledFader\\ledFader.ino"
void setup()
{
    pinMode(14, OUTPUT);

}

void loop()
{
    for (int i = 0; i < 255; i++)
    {
        analogWrite(14, i);
    }

    for (int i = 255; i > 0; i--)
    {
        analogWrite(14, i);
    }
    
}
