#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

RH_ASK driver;

int msg;

void setup()
{
    Serial.begin(9600);   // Debugging only
    if (!driver.init())
         Serial.println("init failed");
}

void loop()
{

	msg = analogRead(A2);

	// Serial.println(msg);
	// delay(100);

    char buffer[5];
  
    dtostrf(msg,5,0,buffer);

    driver.send((uint8_t *)buffer, strlen(buffer));

    driver.waitPacketSent();
    //delay(1000);
}