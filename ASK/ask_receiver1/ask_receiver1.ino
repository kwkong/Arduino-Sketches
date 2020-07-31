#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile

RH_ASK driver; //(uint16_t speed=2000, uint8_t rxPin=11, uint8_t txPin=12, uint8_t pttPin=10, bool pttInverted=false)

void setup()
{
    Serial.begin(9600); // Debugging only
    if (!driver.init())
         Serial.println("init failed");
}

void loop()
{
    uint8_t buf[24];
    uint8_t buflen = sizeof(buf);
    if (driver.recv(buf, &buflen)) // Non-blocking
    {
      int i;
      // Message with a good checksum received, dump it.
      Serial.print("Message: ");
      Serial.println((char*)buf);         
    }
}