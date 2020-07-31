#include <SoftwareSerial.h> 

SoftwareSerial mySerial(3,2);

String msg = "";

int val = 0; 

void setup()
{
	msg.reserve(20);
	Serial.begin(115200);
	mySerial.begin(115200);	
}

void loop()
{
	val = analogRead(A0);
	msg = String(val);
	Serial.println(msg);
	mySerial.println(msg);
	delay(100);
}

