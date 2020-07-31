#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

unsigned long throttle;
unsigned long yaw;
unsigned long msg;

char ack;

char buffer[7];

void setup()
{
    Serial.begin(9600);   // Debugging only
    mySerial.begin(9600);
    pinMode(13, OUTPUT);


}

void loop()
{

	throttle = analogRead(A2);
	throttle = map(throttle, 0, 1024, 80, 55);

	yaw = analogRead(A7);
	yaw = map(yaw, 0, 1023, 55, 130);

	// if ((throttle >  52) && (throttle < 57))
	// digitalWrite(13, HIGH);
	
	// else 
	// digitalWrite(13, LOW);

	throttle = throttle*1000;

	msg = throttle + yaw;


	// mySerial.write(msg);

	 dtostrf(msg,0,0,buffer);

	 mySerial.write(buffer, 7);

	 while (ack != 'k')
	 {
		 if (mySerial.available())
		 ack = mySerial.read();
	 }

	ack = 0;
}
