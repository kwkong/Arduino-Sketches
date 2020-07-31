#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

RH_ASK driver;	//(uint16_t speed=2000, uint8_t rxPin=11, uint8_t txPin=12, uint8_t pttPin=10, bool pttInverted=false)

void setup()
{
    Serial.begin(9600);   // Debugging only
    if (!driver.init())
         Serial.println("init failed");
}

void loop()
{
    const char *msg = "Yekini is a bag of dicks";

    driver.send((uint8_t *)msg, strlen(msg));
    driver.waitPacketSent();
    delay(1000);
}