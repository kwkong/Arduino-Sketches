
const int L2 = 5;
const int L1 = 6;
const int R1 = 10;
const int R2 = 11;

const int qtr2Pin = A1;
const int qtr3Pin = A4;
const int qtr4Pin = A5;
const int qtr5Pin = A2;
const int qtr6Pin = A3;
const int qtr7Pin = A0;

int qtrVal[7];
char qtrState[7];

int brakeState;
       // will store last time LED was updated


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
	//delay(100);
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
		moveForward(140);
		Serial.println("Straight");
	}

	if ((!strcmp(qtrState, "000100")))
	{
		moveForward(140);
		Serial.println("Straight");
	}

	if ((!strcmp(qtrState, "001000")))
	{
		moveForward(140);
		Serial.println("Straight");
	}

	if ((!strcmp(qtrState, "011000")))
	{
		moveLeftTicks(235, 1, 200);
		moveForward(80);
		delay(100);
		moveStop();
		moveRightTicks(235, 1, 200);
	}

	if ((!strcmp(qtrState, "000110")))
	{
		moveRightTicks(235, 1, 200);
		moveForward(80);
		delay(200);
		moveStop();
		moveLeftTicks(235, 1, 200);
	}

	else 
	{
		moveStop();

		while((!strcmp(qtrState, "000000")))
        {
       	 getLine();
       	 Serial.println("I see nothing");
        }

		moveStop();
		Serial.println("Stop");

	}	
}

void followRight()
{
	getLine();

	if ((!strcmp(qtrState, "001111")) || (!strcmp(qtrState, "000111")) || (!strcmp(qtrState, "001111")))    //RIGHT
	{  
		moveForward(80);
		delay(300);
		moveStop();
		delay(1000);
		moveReverse(125);
		delay(50);
		moveStop();
		delay(1000);

       // unsigned long timeout = millis() + 1500;
        // while(millis() < timeout)
        // {

			while((strcmp(qtrState, "000001")) && (strcmp(qtrState, "000011")) && (strcmp(qtrState, "000111")) )
			{
				getLine();
				moveRight(235);
			}

			moveStop();
			delay(1000);

			while((strcmp(qtrState, "001100")))
			{
				getLine();
				moveRight(235);
			}

			moveStop();
			//while(1);

			// if (!strcmp(qtrState, "000001"))
			// {
			// 	moveForward(80);
			// 	delay(100);

			// }
   //              //if(millis() > timeout)
   //              //	break;
			// }

	   //}
        //moveStop();
		//while(1);

		// moveRightTicks(235,5, 300);

		// Serial.println("Right");

		// // if ((!strcmp(qtrState, "000111")) || (!strcmp(qtrState, "000011")) || (!strcmp(qtrState, "000001")))
		// // {
		// 	while(strcmp(qtrState, "001100"))
		// 	{
		// 		getLine();
		// 		moveRightTicks(235,1,200);
		// 		moveForward(80);
		// 		delay(100);
		// 		moveStop();
		// 	}

		// 	//while(1);

		// 	if (strcmp(qtrState,"001100"))
		// 	{
		// 		moveLeftTicks(235, 3, 200);
		// 	}

		// 	while(strcmp(qtrState, "001100"))
		// 	{
		// 		getLine();
		// 		moveLeftTicks(235,1,200);
		// 	}

		// 	while(1);
		// //} 

		// if ((!strcmp(qtrState, "000110")))
		// {
		// 	while (strcmp(qtrState, "001100"))
		// 	{
		// 		getLine();
		// 		moveLeftTicks(235,1,200);
		// 	}
		// 	//while(1);
		// }
		
	}
}

void followLine()
{
	while((strcmp(qtrState, "001111")))
	{
		followStraight();
	}

	followRight();
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