
const int L2 = 5;
const int L1 = 6;
const int R1 = 10;
const int R2 = 11;

int qtr3Pin = A0;
int qtr4Pin = A1;
int qtr5Pin = A2;
int qtr6Pin = A3;

int qtrVal[5];
char qtrState[5];

int brakeState; 

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
}

void loop()
{
	getLine();
	//delay(100);
	followLine();
}

void getLine()
{
	qtrVal[0] = analogRead(qtr3Pin);
	qtrVal[1] = analogRead(qtr4Pin);
	qtrVal[2] = analogRead(qtr5Pin);
	qtrVal[3] = analogRead(qtr6Pin);

	for(int i=0;i<4;i++)
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
		Serial.print("   ");
}

void moveForward(int speed)
{
	analogWrite(L1, speed);
	analogWrite(R1, speed);

	analogWrite(L2, 0);
	analogWrite(R2, 0);
}

void moveLeft(int speed, int tickNum)
{
	for(int i; i<=tickNum;i++)
	{
		analogWrite(L1, speed);
		analogWrite(R2, speed);
		delay(100);
		analogWrite(L2, 0);
		analogWrite(R1, 0);
	}
}

void moveRight(int speed)
{
	analogWrite(L2, speed);
	analogWrite(R1, speed);

	analogWrite(L1, 0);
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

void followLine()
{
	// if ((qtrState[5] == (qtrComp[4] = "1111")) || (qtrState[5] == (qtrComp[4] = "0000")))
	// {
	// 	moveStop();
	// }

	// if ((qtrState[5] == (qtrComp[5] = "0110")) || (qtrState[5] == (qtrComp[5] = "0010"))  || (qtrState[5] == (qtrComp[5] = "0100")))
	// {
	// 	moveForward(65);

	// 	Serial.println("FORWARD");
	// }

	//if (!strcmp(qtrState, "0110"))

	if ((!strcmp(qtrState, "0100")) || (!strcmp(qtrState, "0110"))|| (!strcmp(qtrState, "0010")))
	{
		moveForward(115);
		brakeState = 1;
		Serial.println("Straight");
	}

	if ((!strcmp(qtrState, "1110")) || (!strcmp(qtrState, "1100")))    // 

	{   
		moveStop();
		delay(3000);
		moveForward(200);
		delay(50);
		moveStop();
		delay(3000); 
		moveRight(255);
		//delay(500);
		// moveRight(205);
		// brakeState = 1;
		 Serial.println("Right");

		// while ((strcmp(qtrState, "1000"))) //|| (strcmp(qtrState, "1100"))))
		// {
		// 	getLine();
		// 	moveRight(225);
		// }
        while((strcmp(qtrState, "0000")))
        {
       	 getLine();
       	 Serial.println("I see something");
        }
        moveStop();
		delay(3000);
		moveRight(255);
        while((!strcmp(qtrState, "0000")))
        {
       	 getLine();
       	 Serial.println("I see nothing");
        }

		 moveStop();
        
	    while(1);
	}

	if ((!strcmp(qtrState, "0111")) || (!strcmp(qtrState, "0011")))
	{
		moveLeft(225);
		brakeState = 1;
		//Serial.println("Left");
	}

	else 
	{
		 while((!strcmp(qtrState, "0000")))
        {
       	 getLine();
       	 Serial.println("I see nothing");
        }
		moveStop();

		Serial.println("Stop");

		if (brakeState == 1)
		{
			//moveBrake();
			brakeState = 0;
		}

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
// 		moveRight(255);
// 		delay(500);
	
// 		Serial.println("Right");


// 		//moveStop();
// 		//while(1);
// 		while((!strcmp(qtrState, "0000")));



// 	}
// }