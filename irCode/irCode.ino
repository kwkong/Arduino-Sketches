void setup()
{
	pinMode(4, OUTPUT);
}

void loop()
{

	digitalWrite(4, HIGH);
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(4, LOW);
  digitalWrite(13, LOW);
  delay(1000);
}

