
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
