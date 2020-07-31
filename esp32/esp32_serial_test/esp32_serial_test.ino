
#include <HardwareSerial.h>
 HardwareSerial MySerial(1);

void setup() 
{
   MySerial.begin(115200, SERIAL_8N1, 16, 17); //rx, tx

    Serial.begin(115200);
}

void loop() 
{
    MySerial.print(random(0,100));  //Tx
    MySerial.println(" Send nudes"); 
    delay(1000);  

    // while (MySerial.available() > 0)     //Rx
    // {
    //     char buff = MySerial.read();
    //     Serial.print(buff);
    // }

}