#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

int val;

void setup() 
{
	pinMode(13,OUTPUT); 
	mySerial.begin(9600);
	Serial.begin(9600);
}

void loop()
{
	if (mySerial.available())
	{
	val = mySerial.read();

	Serial.println(val);
	delay(100);
	}

}
