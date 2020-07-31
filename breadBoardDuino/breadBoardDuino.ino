#define LED_PIN 9

void setup()
{
	Serial.begin(9600);	
	pinMode(LED_PIN, OUTPUT);
}

void loop()
{
	digitalWrite(LED_PIN, HIGH);
	delay(1000);
	digitalWrite(LED_PIN, LOW);
	delay(1000);
	Serial.println("send nudes");
}

