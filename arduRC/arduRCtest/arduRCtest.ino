#include <Servo.h>

Servo myMotor;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int speed = 0;    // variable to store the servo position

void setup() {
  myMotor.attach(9);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);
}
7
void loop() {

	if (Serial.available())
	{
		//speed = (Serial.read());
		speed = Serial.parseInt();
	}

    //myMotor.write(speed);

    analogWrite(9, speed);
    Serial.println(speed);
    delay(30);

}
