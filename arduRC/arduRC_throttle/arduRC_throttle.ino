#include <Servo.h>

Servo esc;
int throttlePin = A0;  //Potentiometer Pin
//int throttlePin = A1; 

void setup() {
  esc.attach(9); //ESC Pin
  Serial.begin(9600);
}

void loop() {
 int throttle = analogRead(throttlePin);
  //hthrottle = map(throttle, 650, 1023, 70, 55); //Motor Throttle



 throttle = map(throttle, 0, 1023, 130, 55); //Motor Throttle
  esc.write(throttle);

  Serial.println(throttle);
 // delay(50);
}