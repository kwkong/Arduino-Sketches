#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX


void setup() 
{
	pinMode(7, OUTPUT);
	pinMode(8, OUTPUT);
	pinMode(13, OUTPUT);

	digitalWrite(7, LOW);
	digitalWrite(8, LOW);

	Serial.begin(9600);	
	mySerial.begin(9600);

	Serial.println("Init");

	delay(200);
}

void loop()
{
	if (mySerial.available())
	{
		Serial.write(mySerial.read());
		digitalWrite(13, (!digitalRead(13)));
	}
}


