
#include <RoboClaw.h>

// Create the RoboClaw object, passing the pointer to the hardware serial object
// and the serial timeout value


void setup() {
    roboclaw = RoboClaw(&Serial1, 10000);
// Begin serial communication at the given baudrate
roboclaw.begin(38400);
}

void loop()
{
// Call a method of RoboClaw object to control the motor controller
roboclaw.ForwardM1(0x80, 127);
delay(2000);
roboclaw.ForwardM1(0x80, 0);
delay(2000);
}