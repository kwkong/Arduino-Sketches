

#define blinkPin 23
#define blinkDelay 1000


void setup()
{
	pinMode(blinkPin, OUTPUT);
	Serial.begin(115200);	
}

void loop()
{
	digitalWrite(blinkPin, HIGH);
	Serial.println("Blink");
	delay(blinkDelay);
	digitalWrite(blinkPin, LOW);
	delay(blinkDelay);
}

