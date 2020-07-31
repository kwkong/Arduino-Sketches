#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

int val;

void setup() 
{
	pinMode(13,OUTPUT); 
	mySerial.begin(9600);
}

void loop()
{
	val = mySerial.read();

	if (val == 1)
	{
		digitalWrite(13, HIGH);
	}
	if (val == 0)
	{
		digitalWrite(13, LOW);
	}

}
