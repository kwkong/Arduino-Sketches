#include <Wire.h>
#include <Adafruit_Colour_Sensor.h>
//#include <PWMServo.h>
#include <SoftwareSerial.h>



Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
//SoftwareSerial esp(2,3);

//PWMServo claw;

#define DEBUG true

#define L2 5
#define L1 6
#define R1 12
#define R2 11

#define servoPin 9

#define qtr2Pin A1
#define qtr3Pin A4
#define qtr4Pin A5
#define qtr5Pin A2
#define qtr6Pin A3
#define qtr7Pin A0


String sendColour;
String r = "";

int val;

int openAngle = 65;
int closeAngle = 45;

int qtrVal[7];
char qtrState[7];


void setup()
{
	Serial.begin(9600);

	motorsInit();
	colourSensorInit();	

	//claw.attach(servoPin);

	//sendData("AT+RT\r\n",5000,DEBUG);

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
	driveToStart();
}

void colourSensorInit()
{
	pinMode(4, OUTPUT);
	
	Serial.println("Color View Test!");

    digitalWrite(4, HIGH);
    delay(1000);

    if (tcs.begin())
    {
        Serial.println("Found sensor");
    }

    else
    {
        Serial.println("No TCS34725 found ... check your connections");
        while (1); // halt!
    }

    delay(1000);
    digitalWrite(4, LOW);
}

void motorsInit()
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

// void gripper()
// {

// 	val = sendColour.indexOf('U');

// 	if (sendColour.charAt((val+1)) == 'n')
// 	{
// 		claw.write(openAngle);
// 	}

// 	else
// 	{
// 		claw.write(closeAngle);
// 	}
// }

void getColour()
{
		moveStop();
				
		uint16_t red, green, blue, clear;
	    uint16_t reds[10], greens[10], blues[10];
	    uint16_t redSum, greenSum, blueSum;

	    digitalWrite(4, HIGH);
	    delay(500);

	    for (int i=0; i<10; i++)
	    {
	   		tcs.getRawData(& red, & green, & blue, &clear);

	   		reds[i] = red;
	   		greens[i] = green;
	   		blues[i] = blue;

	   		redSum += reds[i];
	   		greenSum += greens[i];
	   		blueSum += blues[i];

		}

		delay(500);

	    digitalWrite(4, LOW);

		red = redSum / 10;
		green = greenSum / 10;
		blue = blueSum / 10;

		//tcs.getRawData(& red, & green, & blue, &clear);

		//Serial.print("Red: "); Serial.print(red);
		//Serial.print("    Green: "); Serial.print(green);
		//Serial.print("    Blue: "); Serial.println(blue);



	    if ((red > 0) && (red < 200) && (green > 300) && (green < 1000) && (blue > 200) && (blue < 500))
	    {
	        Serial.println("Shit is green yo.");
	    }

	    else if ((red > 0) && (red < 200) && (green > 100) && (green < 300) && (blue > 300) && (blue < 800))
	    {
	        Serial.println("Shit is blue yo.");
	    }

	    else if ((red > 400) && (red < 1000) && (green > 100) && (green < 300) && (blue > 100) && (blue < 300))
	    {
	        Serial.println("Shit is red yo.");
	    }

	    else
	    {
	        Serial.println("idk what that shit is yo");
	    }

	    delay(300);
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

//-------------------------Web Section--------------------------------------//


// String sendData(String command, const int timeout, boolean debug)
// {
//     String response = "";
    
//     esp.print(command); // send the read character to the esp
    
//     long int time = millis();
    
//     while( (time+timeout) > millis())
//     {
//         while(esp.available())
//         {
        
//             // The esp has data so display its output to the serial window 
//             char c = esp.read(); // read the next character.
//             response+=c;
//         }  
//     }
    
//     if(debug)
//     {
//         Serial.print(response);
//     }
    
//     return response;
// }


// void serverRead()
// {
// 	esp.begin(9600);

// 	    if(!esp.available()) // check if the esp is sending a message 
//     {

//         String cipStart = "AT+CIPSTART=\"TCP\",\"192.168.1.2\",80\r\n";
        
//         String params = "esp=";
        
//         String post = "POST /SwarmServer.php HTTP/1.1\r\nHost: 192.168.1.2\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: ";
//         post += params.length();
//         post += "\r\n\r\n";
//         post += params;
//         post += "\r\n\r\n";
        
//         String cipSend = "AT+CIPSEND=";
//         cipSend += post.length();
//         cipSend += "\r\n";
        
//         sendData(cipStart,100,DEBUG);
//         sendData(cipSend,100,DEBUG);        
//         sendData(post,1000,DEBUG);

//         esp.end();

//     }
// }

// void serverWrite()
// {
// 	esp.begin(9600);

// 	if(!esp.available()) // check if the esp is sending a message 
//     {
//     //Serial.println("Enter Message");
//      //while(!Serial.available());

//         //String text = Serial.readStringUntil("\n");

//         String cipStart = "AT+CIPSTART=\"TCP\",\"192.168.1.2\",80\r\n";
        
//         String params = "msg=";
//         params += sendColour;
        
//         String post = "POST /SwarmServer.php HTTP/1.1\r\nHost: 192.168.1.2\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: ";
//         post += params.length();
//         post += "\r\n\r\n";
//         post += params;
//         post += "\r\n\r\n";
        
//         String cipSend = "AT+CIPSEND=";
//         cipSend += post.length();
//         cipSend += "\r\n";
        
//         sendData(cipStart,100,DEBUG);
//         sendData(cipSend,100,DEBUG);        
//         sendData(post,1000,DEBUG);

//         esp.end();
// 	}
// }

//---------------------------------------------------------------------//

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

//----------------------------Straight---------------------------//

	if ((!strcmp(qtrState, "001100"))) //STRAIGHT
	{
		moveForward(65);
		//Serial.println("Straight");
	}

	if ((!strcmp(qtrState, "000100")))
	{
		moveForward(65);
		//Serial.println("Straight");
	}

	if ((!strcmp(qtrState, "001000")))
	{
		moveForward(65);
		//Serial.println("Straight");
	}

//-------------------------Correct Left--------------------------//

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

//-------------------------Correct Right-------------------------//

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

//-----------------------------Stop------------------------------//	

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

	//Drive Straight until you see a Right Turn//

	while((strcmp(qtrState, "011111")) && (strcmp(qtrState, "001111")) && (strcmp(qtrState, "000111")))
	{
		getLine();
		followStraight();
		Serial.println("HEADING FOR RIGHT TURN");

		if (!strcmp(qtrState, "011110"))
		break;

		if(!strcmp(qtrState, "000000"))
		{
			delay(1000);
			moveReverse(80);
			delay(500);
			moveStop();
		}
	}

	if ((!strcmp(qtrState, "001111")) || (!strcmp(qtrState, "000111")))    //Make a right turn
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
			moveRight(250);

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

		while((strcmp(qtrState, "001100")))
		{
			getLine();
			moveRight(235);


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

	while((strcmp(qtrState, "111110")) && (strcmp(qtrState, "111100")) && (strcmp(qtrState, "111000")))
	{
		getLine();
		followStraight();
		Serial.println("HEADING FOR LEFT TURN");

		if(!strcmp(qtrState, "000000"))
		{
			delay(1000);
			moveReverse(80);
			delay(500);
			moveStop();
		}

		if (!strcmp(qtrState, "011110"))
		break;
	}

	if ((!strcmp(qtrState, "111100")) || (!strcmp(qtrState, "111000")))    //Make a Left Turn
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
			moveLeft(250);

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

		while((strcmp(qtrState, "001100")))
		{
			getLine();
			moveLeft(235);
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

	while(strcmp(qtrState,"111111"))
	{
		followStraight();
	}

	while (strcmp(!qtrState, "111111"))
	{
		getColour();
		//serverRead();
		//serverWrite();
		//gripper();
	}
}

void followLine()
{
	getLine();
	followStraight();

	if ((!strcmp(qtrState, "001111")) || (!strcmp(qtrState, "000111"))|| (!strcmp(qtrState, "011111")))
	{
		followRight();
	}

	if ((!strcmp(qtrState, "111100")) || (!strcmp(qtrState, "111000")) || (!strcmp(qtrState, "111110")))
	{
		followLeft();
	}
}
