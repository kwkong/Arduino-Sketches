#include <Servo.h>
#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile



unsigned long msg;
unsigned long throttle;
unsigned long yaw;

unsigned long message;

char *ptr;



Servo servo;
Servo esc;
RH_ASK driver;


void setup()
{
    pinMode(13,HIGH);

    Serial.begin(9600); // Debugging only

    if (!driver.init())
    	Serial.println("init failed");

    servo.attach(3);
    esc.attach(8);


    
     // set the data rate for the SoftwareSerial port
}

void loop()
{

    uint8_t buf[5];
    uint8_t buflen = sizeof(buf);
    if (driver.recv(buf, &buflen)) // Non-blocking
    {
      int i;
      // Message with a good checksum received, dump it.
      //Serial.print("Message: ");
      //Serial.println((char*)buf);

      //msg = atoi(buf);

      //msg = atol(buf);


      msg = strtoul(buf, &ptr, 10);

    }


    

      throttle = msg/1000;
      yaw = msg % 1000;


      // Serial.print("Message: ");
      // Serial.println(msg);        
      // Serial.print("          Yaw: ");
      // Serial.println(yaw);
      // Serial.print("Throttle: ");
      // Serial.print(throttle);

      // delay(500);

      servo.write(yaw);
      esc. write(throttle);

      delay(5);




}      