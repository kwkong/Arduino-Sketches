
#define L2 5
#define L1 6
#define R1 10
#define R2 11

#define qtr2Pin A1
#define qtr3Pin A4
#define qtr4Pin A5
#define qtr5Pin A2
#define qtr6Pin A3
#define qtr7Pin A0

int qtrVal[7];
char qtrState[7];


void setup()
{
	pinMode(L1, OUTPUT);
	pinMode(L2, OUTPUT);
	pinMode(R1, OUTPUT);
	pinMode(R2, OUTPUT);

	analogWrite(L1, 0);
	analogWrite(R1, 0);

	analogWrite(L2, 0);
	analogWrite(R2, 0);

	Serial.begin(9600);	

	while((strcmp(qtrState, "111111")))
	{
		getLine();
	}

	moveForward(130);
	delay(300);
	moveStop();
	delay(1000);
}

void loop()
{
	getLine();
	followLine();
}

void getLine()
{
	qtrVal[0] = analogRead(qtr7Pin);
	qtrVal[1] = analogRead(qtr6Pin);
	qtrVal[2] = analogRead(qtr5Pin);
	qtrVal[3] = analogRead(qtr4Pin);
	qtrVal[4] = analogRead(qtr3Pin);
	qtrVal[5] = analogRead(qtr2Pin);

	for(int i=0;i<6;i++)
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
		//delay(100);
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

void moveRight(int speed)
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

void moveLeft(int speed)
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

	if ((!strcmp(qtrState, "001100"))) //STRAIGHT
	{
		moveForward(65);
		Serial.println("Straight");
	}

	if ((!strcmp(qtrState, "000100")))
	{
		moveForward(65);
		Serial.println("Straight");
	}

	if ((!strcmp(qtrState, "001000")))
	{
		moveForward(65);
		Serial.println("Straight");
	}

	if ((!strcmp(qtrState, "011000")) || (!strcmp(qtrState, "011100")))		//CORRECT LEFT
	{
		moveLeft(200);
		while((strcmp(qtrState, "001100")) && (strcmp(qtrState, "000100")) && (strcmp(qtrState, "001000")))//&& (strcmp(qtrState, "011000")))
		{
			getLine();
			moveLeft(200);
			Serial.println("Correcting Left");
			
		}

		moveStop();
		delay(300);
	}

	if ((!strcmp(qtrState, "000110")) || (!strcmp(qtrState, "001110")))	//CORRECT RIGHT
	{
		while((strcmp(qtrState, "001100")) && (strcmp(qtrState, "000100")) && (strcmp(qtrState, "001000")))//&& (strcmp(qtrState, "011000")))
		{
			getLine();
			moveRight(200);
			Serial.println("Correcting Right");
		}

		moveStop();
		delay(300);
	}

	if  ((!strcmp(qtrState, "000000")) || (!strcmp(qtrState, "111111")))
	{
		moveStop();

		while((!strcmp(qtrState, "000000")))
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

	if ((!strcmp(qtrState, "001111")) || (!strcmp(qtrState, "000111")))    //RIGHT
	{  
		moveForward(80);
		delay(200);
		moveStop();
		delay(300);
		moveReverse(125);
		delay(50);
		moveStop();
		delay(300);

		while((strcmp(qtrState, "000001")) && (strcmp(qtrState, "000011")) && (strcmp(qtrState, "000111")) )
		{
			getLine();
			moveRight(235);
		}

		moveStop();
		delay(300);

		while((strcmp(qtrState, "001100")))
		{
			getLine();
			moveRight(235);
		}

		moveStop();		
	}
}

void followLeft()
{
	getLine();

	if ((!strcmp(qtrState, "111100")) || (!strcmp(qtrState, "111000")))    //RIGHT
	{  
		moveForward(80);
		delay(200);
		moveStop();
		delay(300);
		moveReverse(125);
		delay(50);
		moveStop();
		delay(300);

		while((strcmp(qtrState, "100000")) && (strcmp(qtrState, "110000")) && (strcmp(qtrState, "111000")) )
		{
			getLine();
			moveLeft(235);
		}

		moveStop();
		delay(300);

		while((strcmp(qtrState, "001100")))
		{
			getLine();
			moveLeft(235);
		}

		moveStop();		
	}
}

void followLine()
{
	getLine();
	followStraight();

	// Serial.println("FOLLOWING STRAIGHT");

	// while((strcmp(qtrState, "011111")) && (strcmp(qtrState, "001111")) && (strcmp(qtrState, "000111")))
	// {
	// 	getLine();
	// 	followStraight();
	// 	Serial.println("HEADING FOR FIRST TURN");
	// }

	// followRight();

	// //Serial.println("ALMOST AT THAT WHILE BREH");

	// while((strcmp(qtrState, "111110")) && (strcmp(qtrState, "111100")) && (strcmp(qtrState, "111000")))
	// {
	// 	getLine();
	// 	followStraight();
	// 	Serial.println("HEADING FOR SECOIND TURN");

	// 	if (!strcmp(qtrState, "011110"))
	// 	break;
	// }

	// followLeft();

	// while((strcmp(qtrState, "111110")) && (strcmp(qtrState, "111100")) && (strcmp(qtrState, "111000")))
	// {
	// 	getLine();
	// 	followStraight();
	// 	Serial.println("HEADING FOR THIRD TURN");

	// 	if (!strcmp(qtrState, "011110"))
	// 	break;
	// }

	// followLeft();

	if ((!strcmp(qtrState, "001111")) || (!strcmp(qtrState, "000111"))|| (!strcmp(qtrState, "011111")))
	{
		followRight();
	}

	if ((!strcmp(qtrState, "111100")) || (!strcmp(qtrState, "111000")) || (!strcmp(qtrState, "111110")))
	{
		followLeft();
	}
}

// int checkTurn()
// {
// 	if((!strcmp(qtrState, "1110")) || (!strcmp(qtrState, "1100")))  //check for left-L
// 	{
//        return leftLTurn
// 	}
// 	//check for right L
// 	//check for L T jun
// 	//check for R T junct
// 	//check for Both T junct
// 	//check for 4 way


// }

// int turn(int dir)
// {
// 	if((checkTurn() == leftLTurn) && (dir == left))
// 	{
// 		//delay(00);
// 		moveRightTicks(255);
// 		delay(500);
	
// 		Serial.println("Right");


// 		//moveStop();
// 		//while(1);
// 		while((!strcmp(qtrState, "0000")));



// 	}
// }