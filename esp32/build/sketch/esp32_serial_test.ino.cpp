#include <Arduino.h>
#line 1 "c:\\Arduino\\My Sketches\\esp32\\esp32_serial_test\\esp32_serial_test.ino"
#line 1 "c:\\Arduino\\My Sketches\\esp32\\esp32_serial_test\\esp32_serial_test.ino"

#include <HardwareSerial.h>
 HardwareSerial MySerial(1);

#line 5 "c:\\Arduino\\My Sketches\\esp32\\esp32_serial_test\\esp32_serial_test.ino"
void setup();
#line 12 "c:\\Arduino\\My Sketches\\esp32\\esp32_serial_test\\esp32_serial_test.ino"
void loop();
#line 5 "c:\\Arduino\\My Sketches\\esp32\\esp32_serial_test\\esp32_serial_test.ino"
void setup() 
{
   MySerial.begin(115200, SERIAL_8N1, 16, 17); //rx, tx

    Serial.begin(115200);
}

void loop() 
{
    MySerial.print(random(0,100));
    MySerial.println(" Send nudes"); 
    delay(1000);  

    // while (MySerial.available() > 0) 
    // {
    //     char buff = MySerial.read();
    //     Serial.print(buff);
    // }

}
