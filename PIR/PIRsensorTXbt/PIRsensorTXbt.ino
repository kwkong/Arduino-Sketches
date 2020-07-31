#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11);

int val;
int state;

unsigned long timer;


void setup()
{
	mySerial.begin(38400);
	pinMode(13,HIGH);
}

void loop()
{

		digitalWrite(13, HIGH);
		state = 1;
		mySerial.write(state);
		val = digitalRead(30);	
		timer = millis();

		while (val == 0)
		{	

			val = digitalRead(30);

			if ((millis()-timer) >= 5000)
			{

				state= 0;
				digitalWrite(13, LOW);
				mySerial.write(state);


			}

		}

}