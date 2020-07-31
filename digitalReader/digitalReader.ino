#define inputPin 25

void setup()
{
	pinMode(inputPin, INPUT);
	Serial.begin(115200);	
}

void loop()
{
	if(digitalRead(inputPin))
	{
		Serial.println("Boop");	
	}

	else 
	{
		Serial.println("No boop");
	}

	delay(200);
}

