#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX


void setup() 
{
	pinMode(7, OUTPUT);
	pinMode(8, OUTPUT);
	pinMode(13, OUTPUT);

	digitalWrite(7, HIGH);
	digitalWrite(8, HIGH);

	Serial.begin(9600);	
	mySerial.begin(9600);

	Serial.println("Init");

	delay(200);

	mySerial.write(0xC1);
	mySerial.write(0xC1);
	mySerial.write(0xC1);
}

void loop()
{
	if (mySerial.available())
	{
		int inByte = mySerial.read();

		if (inByte < 0x10)
		Serial.print("0x0");

		else
		Serial.print("0x");

		Serial.print((inByte),HEX);
		Serial.print(" ");
	}
}


