#include <Hercules.h>
#include <Hercules_dfs.h>
#include <Hercules_Stepper.h>
#include <seeed_pwm.h>

#include <RobotMovement4.h>





void setup()
{
	Move.begin(52);
	Serial1.begin(9600);

		//Move.Forward(40);
	// Move.TurnRight(25);
	// delay(1000);
	// Move.Stop();

		 Move.Forward(25);
	 //Move.TurnLeft(25);
	 delay(1000);
}

void loop()
{

	 Move.Stop();
	 delay(1000);
	 	// delay(1000);
	// Move.Stop();

	// Move.TurnRight(25);
	// delay(3000);
	// Move.Stop();
	// delay(3000);
}

