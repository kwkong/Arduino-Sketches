#define CBUTT 26
#define PIR 25
#define CLED 27
#define BUZZ 14


void setup()
{
	pinMode(CBUTT, INPUT_PULLUP);
	pinMode(PIR, INPUT);
	pinMode(CLED, OUTPUT);
	pinMode(BUZZ, OUTPUT);
	Serial.begin(9600);
	Serial.println("AY");
}

void loop()
{
	//digitalWrite(CLED, digitalRead(PIR));
	//digitalWrite(BUZZ, !digitalRead(CBUTT));

	delay(500);
	if (digitalRead(PIR))
	{
		Serial.println("ON");
	}


}

