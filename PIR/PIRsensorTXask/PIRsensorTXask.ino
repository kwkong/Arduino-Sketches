#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

RH_ASK driver;  //(uint16_t speed=2000, uint8_t rxPin=11, uint8_t txPin=12, uint8_t pttPin=10, bool pttInverted=false)

int val;
int state;
char buffer[5];

unsigned long timer;


void setup()
{
	pinMode(13,OUTPUT);
	    Serial.begin(9600); // Debugging only
    if (!driver.init())
         Serial.println("init failed");
}

void loop()
{

		digitalWrite(13, HIGH);
		state = 1;

	    dtostrf(state,5,0,buffer);
	    driver.send((uint8_t *)buffer, strlen(buffer));
	    driver.waitPacketSent();

		val = digitalRead(30);	
		timer = millis();

		while (val == 0)
		{	

			val = digitalRead(30);

			if ((millis()-timer) >= 5000)
			{

				state= 0;
				digitalWrite(13, LOW);

			    dtostrf(state,5,0,buffer);
			    driver.send((uint8_t *)buffer, strlen(buffer));
			    driver.waitPacketSent();

			}

		}

}