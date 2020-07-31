#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

RH_ASK driver;

unsigned long throttle;
unsigned long yaw;
char buffer[6];

unsigned long msg;


void setup()
{
    Serial.begin(9600);   // Debugging only
    if (!driver.init())
         Serial.println("init failed");
}

void loop()
{

	throttle = analogRead(A2);
	throttle = map(throttle, 0, 1023, 70, 55);

	yaw = analogRead(A7);
	yaw = map(yaw, 0, 1023, 130, 55);

	throttle = throttle*1000;

	msg = throttle + yaw;


	// Serial.println(msg);
	// delay(100);


	sendASK(msg);

	// Serial.print(msg);
	// delay(1000);
  
    // dtostrf(throttle,5,0,buffer);

    // driver.send((uint8_t *)buffer, strlen(buffer));

    // driver.waitPacketSent();
    //delay(1000);
}

void sendASK(unsigned long message)
{
	dtostrf(message,5,0,buffer);

    driver.send((uint8_t *)buffer, strlen(buffer));

    driver.waitPacketSent();
}