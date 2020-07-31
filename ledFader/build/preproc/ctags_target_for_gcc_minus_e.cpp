# 1 "c:\\Arduino\\My Sketches\\ledFader\\ledFader.ino"
# 1 "c:\\Arduino\\My Sketches\\ledFader\\ledFader.ino"



void setup()
{
    pinMode(14, 0x02);

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
