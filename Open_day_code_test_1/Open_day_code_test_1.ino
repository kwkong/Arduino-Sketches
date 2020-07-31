#include <NewPing.h>
#include <Servo.h>


#define TRIGGER_PIN  10  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.


Servo gateServo;

#define redLED A2
#define greenLED A1
#define gateServoPin 9 // on gate code, this is 9


#define IN4 9 //on  robot, this is 9   
#define IN3 8
#define IN2 7
#define IN1 3

#define ENA 5
#define ENB 6
#define buzzer 4


void setup()
{ 
	pinMode(redLED, OUTPUT);
	pinMode(greenLED, OUTPUT);
	pinMode(buzzer, OUTPUT);

	pinMode(13, OUTPUT);

	pinMode(IN1, OUTPUT);
	pinMode(IN2, OUTPUT);
	pinMode(IN3, OUTPUT);
	pinMode(IN4, OUTPUT);
	pinMode(ENA, OUTPUT);
	pinMode(ENB, OUTPUT);

	gateServo.attach(gateServoPin);

	Serial.begin(115200);

	beep();
}

void loop() 
{

	//Write code to let drive until it sees the gate.
	//sample test code//
	
	// if (checkSensor(10) == true)
	// {
	// 	move('s');
	// }
	// else
	// {
	// 	move('f');
	// }


	 digitalWrite(redLED, HIGH);


	 // Serial.println(checkSensor(10));
	 // while(1)
	 // {
	 // Serial.println(sonar.ping_cm());
		// }
	 


	 // if (checkSensor(10) == true)
	 // {
	 // 	//Serial.println("sees something");
	 // 	gateServo.write(90);
	 // 	delay(1000);
		// digitalWrite(redLED, LOW);
	 // 	digitalWrite(greenLED, HIGH);
	 // 	delay(5000);
	 // 	gateServo.write(0);
		// digitalWrite(greenLED, LOW);
	 // 	digitalWrite(redLED, HIGH);
	 // }
  //    else
  //    {
  //    	//	Serial.println("sees nothing");
  //    	gateServo.write(0);
  //    }

 move('f');

 if(checkSensor(15)== true)
{
	move ('s');
	beep();
	beep();
	delay(2000);

	if(checkSensor(15)!= true)
	{
		move('f');
	}
	else
	{
		beep();
	}
}

	//sample test code//

    /************************************/

											    // drive forward 


											    // stop infront of gate, beep horn( 500 ms beep)


											    // wait 2 seconds


											    //check for gate again


											    //if there is no gate, drive through


     /***********************************/



   // BONUS POINTS!!! wRITE CODE TO HAVE ROBOT ALWAYS AVOID OBJECTS IN FRONT OF IT

   /*******************************************/


















   /********************************************/







	//

}

boolean checkSensor(int dist)
{
	if(sonar.ping_cm() < dist)
	{
		return 1;
	}

	else

	return 0;
}

void beep()
{
	digitalWrite(buzzer, HIGH);
	delay(100);
	digitalWrite(buzzer, LOW);
}

void move(char dir)
{
	if((dir == 'f')||(dir == 'F'))	//move forward
	{
		digitalWrite(IN1, LOW);
		digitalWrite(IN2, HIGH);

		digitalWrite(IN3, LOW);
		digitalWrite(IN4, HIGH);

		digitalWrite(ENA, HIGH);
		digitalWrite(ENB, HIGH);

		// analogWrite(ENA, 255);
		// analogWrite(ENB, 255);


	}

	if((dir == 'b')||(dir == 'B'))	//move backward
	{
		digitalWrite(IN1, HIGH);
		digitalWrite(IN2, LOW);

		digitalWrite(IN3, HIGH);
		digitalWrite(IN4, LOW);

		digitalWrite(ENA, HIGH);
		digitalWrite(ENB, HIGH);

		// analogWrite(ENA, 255);
		// analogWrite(ENB, 255);

	} 

	if((dir == 'r')||(dir == 'R'))	//move right
	{
		digitalWrite(IN1, HIGH);
		digitalWrite(IN2, LOW);

		digitalWrite(IN3, LOW);
		digitalWrite(IN4, HIGH);

		digitalWrite(ENA, HIGH);
		digitalWrite(ENB, HIGH);

		// analogWrite(ENA, 255);
		// analogWrite(ENB, 255);

	} 

	if((dir == 'l')||(dir == 'L'))	//move left
	{
		digitalWrite(IN1, LOW);
		digitalWrite(IN2, HIGH);

		digitalWrite(IN3, HIGH);
		digitalWrite(IN4, LOW);

		digitalWrite(ENA, HIGH);
		digitalWrite(ENB, HIGH);

		// analogWrite(ENA, 255);
		// analogWrite(ENB, 255);

	} 

	if((dir == 's')||(dir == 'S'))	//stop
	{
		digitalWrite(IN1, HIGH);
		digitalWrite(IN2, LOW);

		digitalWrite(IN3, HIGH);
		digitalWrite(IN4, LOW);

		digitalWrite(ENA, HIGH);
		digitalWrite(ENB, HIGH);

		delay(100);

		digitalWrite(ENA, LOW);
		digitalWrite(ENB, LOW);
	}
}


/* How to  use code examples for driving robot*/
/*

beep(); // beeps horn
checkSensor(10); //this == 1 if the object is 
                 //less than 10cm from the sensor and 0 if it is not
move('f'); // makes the robot move forward
move('r'); // makes the robot turn right
move('l'); // makes the robot turn left
move('b'); // makes the robot revesrse /go backward
move('s'); // makes the robot stop
delay(500); // delays the code for 500 milliseconds
digitalWrite(redLED, HIGH); // turns red led on


*/
/* How to  use code examples  for gate robot*/

/*sample test code//
	
	if (checkSensor(10) == true)
	{
		move('s');
	}
	else
	{
		move('f');
	}
//sample test code*/
/*sample test code//
	
	if (checkSensor(10) == true)
	{
		gateServo.write(90);

	}
	else
	{
		gateServo.write(0);
	}
//sample test code*/


