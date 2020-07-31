# 1 "c:\\Arduino\\My Sketches\\esp32\\esp32_serial_test\\esp32_serial_test.ino"
# 1 "c:\\Arduino\\My Sketches\\esp32\\esp32_serial_test\\esp32_serial_test.ino"

# 3 "c:\\Arduino\\My Sketches\\esp32\\esp32_serial_test\\esp32_serial_test.ino" 2
 HardwareSerial MySerial(1);

void setup()
{
   MySerial.begin(115200, 0x800001c, 16, 17); //rx, tx

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
