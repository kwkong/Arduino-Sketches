# 1 "c:\\Arduino\\My Sketches\\digitalWrite\\digitalWrite.ino"
# 1 "c:\\Arduino\\My Sketches\\digitalWrite\\digitalWrite.ino"


#define blinkPin 16
#define blinkDelay 1000


void setup()
{
 pinMode(16, 0x02);
 Serial.begin(9600);
}

void loop()
{
 digitalWrite(16, 0x1);
 Serial.println("Blink");
 delay(1000);
 digitalWrite(16, 0x0);
 delay(1000);
}
