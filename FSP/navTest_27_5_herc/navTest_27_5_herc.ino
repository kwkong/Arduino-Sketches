#include <Hercules.h>
#include <Hercules_dfs.h>
#include <Hercules_Stepper.h>
#include <seeed_pwm.h>

#include <RobotMovement4.h>

#define qtr1Pin A0
#define qtr2Pin A1
#define qtr3Pin A2
#define qtr4Pin A3
#define qtr5Pin A5
#define qtr6Pin A4
#define qtr7Pin A6
#define qtr8Pin A7

int qtrVal[9];
char qtrState[9];

void setup()
{
	Move.begin(52);
	Serial.begin(115200);
	Serial1.begin(9600);	

	moveToLine();
}

void loop()
{
	getLine();
	followRight();
	followLeft();
	followLeft();
	followRight();
	followStraight();
}

void getLine()
{
	qtrVal[0] = analogRead(qtr1Pin);
	qtrVal[1] = analogRead(qtr2Pin);
	qtrVal[2] = analogRead(qtr3Pin);
	qtrVal[3] = analogRead(qtr4Pin);
	qtrVal[4] = analogRead(qtr5Pin);
	qtrVal[5] = analogRead(qtr6Pin);
	qtrVal[6] = analogRead(qtr7Pin);
	qtrVal[7] = analogRead(qtr8Pin);

	for(int i=0;i<8;i++)
	{
		if (qtrVal[i] >= 250)
		{
			qtrState[i] = '1';
		}

		else 
		{
			qtrState[i] = '0';
		}

		Serial.write(qtrState[i]);
		delay(7);
	}
		Serial.println("   ");
}

void moveToLine()
{
	while((strcmp(qtrState, "11111111")))
	{
		getLine();
	}

	Move.Forward(15);
	delay(300);
	Move.Stop();
	delay(1000);
}

void followStraight()
{
	getLine();

	//Straight

	if ((!strcmp(qtrState, "00011000")) || (!strcmp(qtrState, "00001000")) || (!strcmp(qtrState, "00010000")))
	{
		Move.Forward(10);
		Serial.println("Straight");
	}

	//Correct Left

	if ((!strcmp(qtrState, "00110000")) || (!strcmp(qtrState, "00111000")) || (!strcmp(qtrState, "01100000")) || (!strcmp(qtrState, "00100000")))		//CORRECT LEFT
	{
		Move.Stop();
		delay(500);
		unsigned long timer = millis();

		while((strcmp(qtrState, "00011000")) && (strcmp(qtrState, "00001000")) && (strcmp(qtrState, "00010000")) && (strcmp(qtrState, "00000111")) && (strcmp(qtrState, "00001111")) && (strcmp(qtrState, "00011111")) && (strcmp(qtrState, "11100000")) && (strcmp(qtrState, "11110000")) && (strcmp(qtrState, "11111000")) )
		{
			getLine();
			Move.TurnLeft(15);
			Serial.println("Correcting Left");	
		}

		Move.Stop();
		delay(750);
	}

	//Correct Right

	if ((!strcmp(qtrState, "00001100")) || (!strcmp(qtrState, "00011100")) || (!strcmp(qtrState, "00000110")) || (!strcmp(qtrState, "00000100")))	//CORRECT RIGHT
	{
		Move.Stop();
		delay(500);

		while((strcmp(qtrState, "00011000")) && (strcmp(qtrState, "00001000")) && (strcmp(qtrState, "00010000")) && (strcmp(qtrState, "11100000")) && (strcmp(qtrState, "11110000")) && (strcmp(qtrState, "11111000")) && (strcmp(qtrState, "00000111")) && (strcmp(qtrState, "00001111")) & (strcmp(qtrState, "00011111")))
		{
			getLine();
			Move.TurnRight(15);
			Serial.println("Correcting Right");
		}

		Move.Stop();
		delay(750);
	}

	//Stop	

	if  ((!strcmp(qtrState, "00000000")))
	{
		Move.Stop();

		while((!strcmp(qtrState, "00000000")))
        {
        	getLine();
        	Move.Reverse(10);
        }

		Move.Stop();
		Serial.println("Stop");
	}	

	if (!strcmp(qtrState, "11111111"))
	{
	    while(1)
		{
	       	 getLine();
	       	 Serial.print("I see nothing    ");
	   	}
	}
}

void followRight()
{
	getLine();

	//Drive Straight until you see a Right Turn//

	while((strcmp(qtrState, "00111111")) && (strcmp(qtrState, "00011111")) && (strcmp(qtrState, "00001111")) && (strcmp(qtrState, "00011110")) )
	{
		getLine();
		followStraight();
		Serial.println("HEADING FOR RIGHT TURN");
	}

	Move.Stop();
	Move.Reverse(20);
	delay(50);
	Move.Stop();
	delay(750);

	if ((!strcmp(qtrState, "00011111")) || (!strcmp(qtrState, "00001111")))    //Make a right turn
	{  
		Move.Stop();
		delay(500);
		Move.Forward(15);
		delay(400);
		Move.Stop();
		delay(500);
		//Move.Reverse(100);
		delay(50);
		Move.Stop();
		delay(500);

		int rightCheck = 0;

		while((strcmp(qtrState, "00000001")) && (strcmp(qtrState, "00000011")) && (strcmp(qtrState, "00000111")))
		{
			getLine();
			Move.TurnRight(20);

			Serial.println("TURNING RIGHT");
		}

		while((strcmp(qtrState, "00011000")) && (strcmp(qtrState, "00001000")) && (strcmp(qtrState, "00010000")))
		{
			getLine();
			Move.TurnRight(20);
		}

		Move.Stop();
		delay(500);
	}
}

void followLeft()
{
	getLine();

	//Drive Straight until you see a Left Turn//

	while((strcmp(qtrState, "11111100")) && (strcmp(qtrState, "11111000")) && (strcmp(qtrState, "11110000")) && (strcmp(qtrState, "01111000")) )
	{
		getLine();
		followStraight();
		Serial.println("HEADING FOR LEFT TURN");
	}

	Move.Stop();
	Move.Reverse(20);
	delay(50);
	Move.Stop();
	delay(750);

	if ((!strcmp(qtrState, "11111000")) || (!strcmp(qtrState, "11110000")))    //Make a left turn
	{  
		Move.Stop();
		delay(500);
		Move.Forward(15);
		delay(400);
		Move.Stop();
		delay(500);
		//Move.Reverse(100);
		delay(50);
		Move.Stop();
		delay(500);

		while((strcmp(qtrState, "10000000")) && (strcmp(qtrState, "11000000")) && (strcmp(qtrState, "11100000")))
		{
			getLine();
			Move.TurnLeft(20);

			Serial.println("TURNING LEFT");
		}

		while((strcmp(qtrState, "00011000")) && (strcmp(qtrState, "00001000")) && (strcmp(qtrState, "00010000")))
		{
			getLine();
			Move.TurnLeft(20);
		}

		Move.Stop();
		delay(500);
	}
}

