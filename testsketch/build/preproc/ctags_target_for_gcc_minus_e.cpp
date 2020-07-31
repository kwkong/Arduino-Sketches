# 1 "c:\\Arduino\\My Sketches\\testsketch\\testsketch.ino"
# 1 "c:\\Arduino\\My Sketches\\testsketch\\testsketch.ino"

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
