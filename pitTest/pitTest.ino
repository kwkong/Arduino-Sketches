void setup()
{
	pinMode(13, OUTPUT);
}

void loop()
{
	digitalWrite(13, digitalRead(6));
}

