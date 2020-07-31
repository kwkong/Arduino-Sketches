
#define L2 3
#define L1 4
#define R1 5
#define R2 6

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
	Serial.begin(115200);

	initMotors();
	moveToLine();

}

void loop()
{
	getLine();
	driveToStart();
}

void initMotors()
{
	pinMode(L1, OUTPUT);
	pinMode(L2, OUTPUT);
	pinMode(R1, OUTPUT);
	pinMode(R2, OUTPUT);

	analogWrite(L1, 0);
	analogWrite(R1, 0);

	analogWrite(L2, 0);
	analogWrite(R2, 0);
}

void moveToLine()
{
	while((strcmp(qtrState, "11111111")))
	{
		getLine();
	}

	moveForward(130);
	delay(300);
	moveStop();
	delay(1000);
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

void moveForward(int speed)
{
	analogWrite(L1, speed);
	analogWrite(R1, speed);

	analogWrite(L2, 0);
	analogWrite(R2, 0);
}

void moveStop()
{
	analogWrite(L1, 0);
	analogWrite(R1, 0);

	analogWrite(L2, 0);
	analogWrite(R2, 0);
}

void moveReverse(int speed)
{
	analogWrite(L1, 0);
	analogWrite(R1, 0);

	analogWrite(L2, speed);
	analogWrite(R2, speed);
}

void moveLeft(int speed)
{
	analogWrite(L1, speed);
	analogWrite(R2, speed);
	analogWrite(L2, 0);
	analogWrite(R1, 0);
}

void moveRightTicks(int speed, int tickNum, int interval)
{
	//unsigned long previousMillis = 0; 

	for(int i=0; i<(tickNum);i++)
	{

		moveRight(speed);

		delay(interval);

		moveStop();

		delay(1000);
	}
}

void moveRight(int speed)
{
	analogWrite(L2, speed);
	analogWrite(R1, speed);
	analogWrite(L1, 0);
	analogWrite(R2, 0);
}

void moveLeftTicks(int speed, int tickNum, int interval)
{
	for(int i=0; i<(tickNum);i++)
	{
		moveLeft(speed);

		delay(interval);

		moveStop();

		delay(1000);
	}
}


void followStraight()
{
	getLine();

//----------------------------Straight---------------------------//

	if ((!strcmp(qtrState, "00011000"))) //STRAIGHT
	{
		moveForward(65);
		Serial.println("Straight");
	}

	if ((!strcmp(qtrState, "00001000")))
	{
		moveForward(65);
		Serial.println("Straight");
	}

	if ((!strcmp(qtrState, "00010000")))
	{
		moveForward(65);
		Serial.println("Straight");
	}

//-------------------------Correct Left--------------------------//

	if ((!strcmp(qtrState, "00110000")) || (!strcmp(qtrState, "00111000")))		//CORRECT LEFT
	{
		moveLeft(200);
		while((strcmp(qtrState, "00011000")) && (strcmp(qtrState, "00001000")) && (strcmp(qtrState, "00010000")))//&& (strcmp(qtrState, "011000")))
		{
			getLine();
			moveLeft(200);
			Serial.println("Correcting Left");
			
		}

		moveStop();
		delay(750);
	}

//-------------------------Correct Right-------------------------//

	if ((!strcmp(qtrState, "00001100")) || (!strcmp(qtrState, "00011100")))	//CORRECT RIGHT
	{
		while((strcmp(qtrState, "00011000")) && (strcmp(qtrState, "00001000")) && (strcmp(qtrState, "00010000")))//&& (strcmp(qtrState, "011000")))
		{
			getLine();
			moveRight(200);
			Serial.println("Correcting Right");
		}

		moveStop();
		delay(750);
	}

//-----------------------------Stop------------------------------//	

	if  ((!strcmp(qtrState, "00000000")) || (!strcmp(qtrState, "11111111")))
	{
		moveStop();

		while((!strcmp(qtrState, "00000000")))
        {
        	while(1)
        	{
		       	 getLine();
		       	 Serial.print("I see nothing    ");
	       	}
        }

		moveStop();
		Serial.println("Stop");

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

		if (!strcmp(qtrState, "00111100"))
		break;

		if(!strcmp(qtrState, "00000000"))
		{
			delay(1000);
			moveReverse(80);
			delay(500);
			moveStop();
		}
	}

	if ((!strcmp(qtrState, "00011111")) || (!strcmp(qtrState, "00001111")))    //Make a right turn
	{  
		moveStop();
		delay(500);
		moveForward(80);
		delay(400);
		moveStop();
		delay(500);
		moveReverse(125);
		delay(50);
		moveStop();
		delay(500);

		int rightCheck = 0;

		while((strcmp(qtrState, "00000001")) && (strcmp(qtrState, "00000011")) && (strcmp(qtrState, "00000111")) )
		{
			getLine();
			moveRight(255);

			if (rightCheck == 0)
			{
				delay(750);
				moveStop();
				delay(500);
				moveForward(80);
				delay(300);
				moveStop();
				delay(500);
				rightCheck = 1;
			}

			Serial.println("TURNING RIGHT");

			// unsigned long 
			// if(!strcmp(qtrState, "000000"))
			// {
			// 	moveStop();
			// 	delay(1000);
			// 	moveReverse(80);
			// 	delay(200);
			// 	moveStop();

			// 	while (strcmp(qtrState, "001100"))
			// 	{
			// 		getLine();
			// 		moveLeft(235);
			// 	}

			// 	break;
			// }
		}

		moveStop();
		delay(300);

		rightCheck = 0;

		while((strcmp(qtrState, "00011000")) && (strcmp(qtrState, "00111000")) && (strcmp(qtrState, "00011100")) && (strcmp(qtrState, "00111100")) && (strcmp(qtrState, "00001100")) && (strcmp(qtrState, "00110000")))
		{
			getLine();
			moveRight(255);

			if (rightCheck == 0)
			{
				delay(300);
				moveStop();
				delay(500);
				moveForward(80);
				delay(400);
				moveStop();
				delay(500);
				rightCheck = 1;
			}


			// if(!strcmp(qtrState, "000000"))
			// {
			// 	moveForward(80);
			// 	delay(400);
			// 	moveStop();

			// 	while (strcmp(qtrState, "001100"))
			// 	{
			// 		getLine();
			// 		moveLeft(235);
			// 	}

			// 	break;
			// }
		}

		moveStop();		
	}
}

void followLeft()
{
	getLine();

	//Drive Straight until you see a Left Turn//

	while((strcmp(qtrState, "11111100")) && (strcmp(qtrState, "11111000")) && (strcmp(qtrState, "11110000")))
	{
		getLine();
		followStraight();
		Serial.println("HEADING FOR LEFT TURN");

		if(!strcmp(qtrState, "00000000"))
		{
			delay(1000);
			moveReverse(80);
			delay(500);
			moveStop();
		}

		if (!strcmp(qtrState, "00111100"))
		break;
	}

	if ((!strcmp(qtrState, "11111000")) || (!strcmp(qtrState, "11110000")))    //Make a Left Turn
	{  
		moveStop();
		delay(1000);
		moveForward(80);
		delay(400);
		moveStop();
		delay(500);
		moveReverse(125);
		delay(50);
		moveStop();
		delay(500);

		int leftCheck = 0;

		while((strcmp(qtrState, "10000000")) && (strcmp(qtrState, "11000000")) && (strcmp(qtrState, "11100000")) )
		{
			getLine();
			moveLeft(255);

			if (leftCheck == 0)
			{
				delay(750);
				moveStop();
				delay(500);
				moveForward(80);
				delay(300);
				moveStop();
				delay(500);
				leftCheck = 1;
			}

			Serial.println("TURNING LEFT");

			// if(!strcmp(qtrState, "000000"))
			// {
			// 	moveStop();
			// 	delay(1000);
			// 	moveReverse(80);
			// 	delay(200);
			// 	moveStop();

			// 	while (strcmp(qtrState, "001100"))
			// 	{
			// 		getLine();
			// 		moveLeft(235);
			// 	}

			// 	break;
			// }
		}
		

		moveStop();
		delay(300);

		leftCheck = 0;

		while((strcmp(qtrState, "00011000")) && (strcmp(qtrState, "00111000")) && (strcmp(qtrState, "00011100")) && (strcmp(qtrState, "00111100")) && (strcmp(qtrState, "00001100")) && (strcmp(qtrState, "00110000")))
		{
			getLine();
			moveLeft(255);

			if (leftCheck == 0)
			{
				delay(300);
				moveStop();
				delay(500);
				moveForward(80);
				delay(400);
				moveStop();
				delay(500);
				leftCheck = 1;
			}
		}

		moveStop();		
	}
}

void driveToStart()
{
	followRight();
	followLeft();
	followLeft();
	followLeft();

	followStraight();
}

void followLine()
{
	getLine();
	followStraight();

	if ((!strcmp(qtrState, "00011111")) || (!strcmp(qtrState, "00001111"))|| (!strcmp(qtrState, "00111111")))
	{
		followRight();
	}

	if ((!strcmp(qtrState, "11111000")) || (!strcmp(qtrState, "11110000")) || (!strcmp(qtrState, "11111100")))
	{
		followLeft();
	}
}
