#define bluSerial Serial3

int msg;

void setup() 
{
	Serial.begin(9600);
	bluSerial.begin(38400);
}

void loop()
{
	if (bluSerial.available())
	{
		msg = bluSerial.read();

		if msg = 
	}

}