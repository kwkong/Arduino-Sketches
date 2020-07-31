
int val;
int sensorPin = 7;

unsigned long timer;


void setup()
{
	pinMode(13,HIGH);
}

void loop()
{

		digitalWrite(13, HIGH);
		val = digitalRead(sensorPin);	
		timer = millis();

		while (val == 0)
		{	

			val = digitalRead(sensorPin);

			if ((millis()-timer) >= 5000)
			{
				digitalWrite(13, LOW);
			}

		}

}

