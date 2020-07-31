
#define L2 4
#define L1 3
#define R1 6
#define R2 5

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

	// moveForward(70);
	// delay(1000);
	// moveStop();


	//followStraight();
	//moveToLine();

}

void loop()
{
	//getLine();
	followStraight();

	//driveToStart();
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

	moveForward(80);
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
	analogWrite(L1, 0);
	analogWrite(R1, 0);

	analogWrite(L2, speed);
	analogWrite(R2, speed);
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
	analogWrite(L1, speed);
	analogWrite(R1, speed);

	analogWrite(L2, 0);
	analogWrite(R2, 0);
}

void moveLeft(int speed)
{
	analogWrite(L1, speed);
	analogWrite(R2, speed);
	analogWrite(L2, 0);
	analogWrite(R1, 0);
}

void turnLeft()
{
	analogWrite(L1, 255);
	analogWrite(R1, 0);

	analogWrite(L2, 0);
	analogWrite(R2, 50);
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

void turnRight()
{
	analogWrite(L1, 0);
	analogWrite(R1, 255);

	analogWrite(L2, 50);
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

	//Straight

	if ((!strcmp(qtrState, "00011000")) || (!strcmp(qtrState, "00001000")) || (!strcmp(qtrState, "00010000")))
	{
		moveForward(80);
		Serial.println("Straight");
	}

	//Correct Left

	if ((!strcmp(qtrState, "00110000")) || (!strcmp(qtrState, "00111000")) || (!strcmp(qtrState, "01100000")) || (!strcmp(qtrState, "00100000")))		//CORRECT LEFT
	{
		moveStop();
		delay(1000);

		while((strcmp(qtrState, "00011000")) && (strcmp(qtrState, "00001000")) && (strcmp(qtrState, "00010000")))//&& (strcmp(qtrState, "011000")))
		{

			getLine();
			moveLeft(80);
			Serial.println("Correcting Left");
			
		}

		moveStop();
		delay(750);
	}

	//Correct Right

	if ((!strcmp(qtrState, "00001100")) || (!strcmp(qtrState, "00011100")) || (!strcmp(qtrState, "00000110")) || (!strcmp(qtrState, "00000100")))	//CORRECT RIGHT
	{
		moveStop();
		delay(1000);

		while((strcmp(qtrState, "00011000")) && (strcmp(qtrState, "00001000")) && (strcmp(qtrState, "00010000")))//&& (strcmp(qtrState, "011000")))
		{
			getLine();
			moveRight(80);
			Serial.println("Correcting Right");
		}

		moveStop();
		delay(750);
	}

	//Stop	

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
	while((strcmp(qtrState, "00001111")) && (strcmp(qtrState, "00000111")) && (strcmp(qtrState, "00011111")))
	{
		followStraight();
	}

	moveStop();
	delay(500);

	moveReverse(80);
	delay(1500);

	moveStop();
	delay(500);

	while((strcmp(qtrState, "00011000")) && (strcmp(qtrState, "00011100")) && (strcmp(qtrState, "00111000")))
	{
		getLine();
		moveRight(80);

		//Serial.println("TURNING RIGHT");
	}

	moveStop();
	delay(500);

}

void followLeft()
{
	while((strcmp(qtrState, "11110000")) && (strcmp(qtrState, "11111000")) && (strcmp(qtrState, "11100000")))
	{
		followStraight();
	}

	moveStop();
	delay(1000);

	moveLeft(80);
	delay(500);
	moveStop();
	delay(1000);

	moveForward(80);
	delay(600);
	moveStop();
	delay(500);
}

void driveToStart()
{
	//followRight();
	//followLeft();
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
