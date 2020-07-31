#include <Hercules.h>
#include <Hercules_dfs.h>
#include <Hercules_Stepper.h>
#include <seeed_pwm.h>
#include <RobotMovement4.h>
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_Colour_Sensor.h>

#define qtr1Pin A0
#define qtr2Pin A1
#define qtr3Pin A2
#define qtr4Pin A3
#define qtr5Pin A5
#define qtr6Pin A4
#define qtr7Pin A6
#define qtr8Pin A7

#define esp Serial2
#define SERVER_IP 192.168.1.2
#define DEBUG true

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

Servo gripper;

int qtrVal[9];
char qtrState[9];

int leftCount = 0;
int rightCount = 0;


String currentDir;
char path[5];
int readStatus = 0;
int val = 0;

char pkgColour[5];
char pkgTarget[3];
int pkgCount = 0;

uint16_t red, green, blue, clear;
uint16_t reds[10], greens[10], blues[10];
uint16_t redSum, greenSum, blueSum;

String response = "";



void setup()
{
	Move.begin(52);
	Serial.begin(115200);
	Serial1.begin(9600);
	gripper.attach(7);

	initColor();
	openGripper();
	moveToLine();
	readPackages();
	getPackages();


}

void loop()
{
	//debugColour();
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
	getLine();

	Move.Stop();

	if (strcmp(qtrState, "11111111"))
	{
		while(1)
		{
			Serial.print("Begin Aligning:       ");
			getLine();
		}
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
		// Move.Stop();
		// delay(100);

		while((strcmp(qtrState, "00011000")) && (strcmp(qtrState, "00001000")) && (strcmp(qtrState, "00010000")) && (strcmp(qtrState, "00000111")) && (strcmp(qtrState, "00001111")) && (strcmp(qtrState, "00011111")) && (strcmp(qtrState, "11100000")) && (strcmp(qtrState, "11110000")) && (strcmp(qtrState, "11111000")) && (strcmp(qtrState, "11111111"))&& (strcmp(qtrState, "01111111")) && (strcmp(qtrState, "11111110")) )
		{
			getLine();
			Move.TurnLeft(20);
			Serial.println("Correcting Left");	
		}

		Move.Stop();
		delay(200);
	}

	//Correct Right

	if ((!strcmp(qtrState, "00001100")) || (!strcmp(qtrState, "00011100")) || (!strcmp(qtrState, "00000110")) || (!strcmp(qtrState, "00000100")))	//CORRECT RIGHT
	{
		// Move.Stop();
		// delay(100);

		while((strcmp(qtrState, "00011000")) && (strcmp(qtrState, "00001000")) && (strcmp(qtrState, "00010000")) && (strcmp(qtrState, "11100000")) && (strcmp(qtrState, "11110000")) && (strcmp(qtrState, "11111000")) && (strcmp(qtrState, "00000111")) && (strcmp(qtrState, "00001111")) && (strcmp(qtrState, "00011111")) && (strcmp(qtrState, "11111111"))&& (strcmp(qtrState, "01111111")) && (strcmp(qtrState, "11111110")))
		{
			getLine();
			Move.TurnRight(20);
			Serial.println("Correcting Right");
		}

		Move.Stop();
		delay(200);
	}
}

void followRight(int turnNum)
{
	getLine();

	int turnCount = 0;

	//Drive Straight until you see a Right Turn//

	while((turnCount+1) <= turnNum)
	{
		getLine();

		while((strcmp(qtrState, "00111111")) && (strcmp(qtrState, "00011111")) && (strcmp(qtrState, "00001111")) && (strcmp(qtrState, "00011110"))  && (strcmp(qtrState, "11111111")) && (strcmp(qtrState, "01111111")) && (strcmp(qtrState, "11111110")))
		{
			getLine();
			followStraight();
			Serial.println("HEADING FOR RIGHT TURN");
		}

		turnCount++;

		if (turnCount != turnNum)
		{
			Move.Forward(20);
			delay(200);
			Serial.println("NOT THAT TURN");
		}
	}

	Move.Stop();
	Move.Reverse(20);
	delay(50);
	Move.Stop();
	delay(300);

	if ((!strcmp(qtrState, "00111111")) || (!strcmp(qtrState, "00011111")) || (!strcmp(qtrState, "00001111")) || (strcmp(qtrState, "11111111")) || (strcmp(qtrState, "01111111")) || (strcmp(qtrState, "11111110")))    //Make a right turn
	{  
		Move.Stop();
		delay(200);
		Move.Forward(10);
		delay(400);
		Move.Stop();
		delay(200);

		while((strcmp(qtrState, "00000001")) && (strcmp(qtrState, "00000011")) && (strcmp(qtrState, "00000111")))
		{
			getLine();
			Move.TurnRight(25);

			Serial.println("TURNING RIGHT");
		}

		Move.Stop();
		delay(200);
		Move.Forward(20);
		delay(100);
		Move.Stop();
		delay(200);

		while((strcmp(qtrState, "00011000")) && (strcmp(qtrState, "00001000")) && (strcmp(qtrState, "00010000")))
		{
			getLine();
			Move.TurnRight(25);
		}

		Move.Stop();
		delay(200);
	}
}

void followLeft(int turnNum)
{
	getLine();

	int turnCount = 0;

	//Drive Straight until you see a Left Turn//

	while((turnCount+1) <= turnNum)
	{
		getLine();

		while((strcmp(qtrState, "11111100")) && (strcmp(qtrState, "11111000")) && (strcmp(qtrState, "11110000")) && (strcmp(qtrState, "01111000")) && (strcmp(qtrState, "11111111")) && (strcmp(qtrState, "01111111")) && (strcmp(qtrState, "11111110")))
		{
			getLine();
			followStraight();
			Serial.println("HEADING FOR LEFT TURN");
		}

		turnCount++;

		if (turnCount != turnNum)
		{
			Move.Forward(20);
			delay(200);
			Serial.println("NOT THAT TURN");
		}
	}

	Move.Stop();
	Move.Reverse(20);
	delay(50);
	Move.Stop();
	delay(300);

	if ((!strcmp(qtrState, "11111100")) || (!strcmp(qtrState, "11111000")) || (!strcmp(qtrState, "11110000")) || (strcmp(qtrState, "11111111")) || (strcmp(qtrState, "01111111")) || (strcmp(qtrState, "11111110")))    //Make a left turn
	{  
		Move.Stop();
		delay(200);
		Move.Forward(10);
		delay(400);
		Move.Stop();
		delay(200);
		Move.Stop();
		delay(200);

		while((strcmp(qtrState, "10000000")) && (strcmp(qtrState, "11000000")) && (strcmp(qtrState, "11100000")))
		{
			getLine();
			Move.TurnLeft(25);

			Serial.println("TURNING LEFT");
		}

		Move.Stop();
		delay(200);
		Move.Forward(20);
		delay(100);
		Move.Stop();
		delay(200);

		while((strcmp(qtrState, "00011000")) && (strcmp(qtrState, "00001000")) && (strcmp(qtrState, "00010000")))
		{
			getLine();
			Move.TurnLeft(25);
		}

		Move.Stop();
		delay(200);
	}
}

void driveToStop()
{
	getLine();

	while ((strcmp(qtrState, "11111111")) && (strcmp(qtrState, "11111110")) && (strcmp(qtrState, "01111111")))
	{
		followStraight();
	}

	Move.Stop();
	Move.Reverse(20);
	delay(50);
	Move.Stop();
}

void turn180()
{
	Move.Reverse(20);
	delay(200);

	getLine();

	while((strcmp(qtrState, "10000000")) && (strcmp(qtrState, "11000000")) && (strcmp(qtrState, "11100000")))
	{
		getLine();
		Move.TurnLeft(25);

		Serial.println("DOING A 180");
	}

	Move.Stop();
	delay(200);
	Move.Reverse(20);
	delay(100);
	Move.Stop();

	getLine();

	while((strcmp(qtrState, "00011000")) && (strcmp(qtrState, "00001000")) && (strcmp(qtrState, "00010000")))
	{
		getLine();
		Move.TurnLeft(25);
	}

	Move.Stop();
}

void getReady()
{
	Move.Stop();
	delay(500);

	unsigned long timer = millis();

	while((millis() - timer) <= 3000)
	{
		followStraight();
	}

	Move.Stop();
	delay(200);
	turn180();

	Move.Stop();
}

void goHome()
{
	turn180();
	Move.Stop();
}

String sendData(String command, const int timeout, boolean debug)
{
	response = "";
    
    esp.print(command); // send the read character to the esp
    
    long int time = millis();
    
    while( (time+timeout) > millis())
    {
        while(esp.available())
        {
        
            // The esp has data so display its output to the serial window 
            char c = esp.read(); // read the next character.
            response+=c;
        }  
    }

	//Serial.print("PATH 0 IS:              ");
	//Serial.println(path[0]);
    
    if(debug)
    {
        Serial.print(response);
    }
    
    return response;
}

void serverRead()
{
	Serial.println("Getting Directions from Server :)");

	esp.begin(9600);

	    if(!esp.available()) // check if the esp is sending a message 
    {

        String cipStart = "AT+CIPSTART=\"TCP\",\"192.168.1.2\",80\r\n";
        
        String params = "esp=";
        
        String post = "POST /SwarmServer.php HTTP/1.1\r\nHost: 192.168.1.2\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: ";
        post += params.length();
        post += "\r\n\r\n";
        post += params;
        post += "\r\n\r\n";
        
        String cipSend = "AT+CIPSEND=";
        cipSend += post.length();
        cipSend += "\r\n";
        
        sendData(cipStart,100,DEBUG);
        sendData(cipSend,100,DEBUG);        
        sendData(post,1000,DEBUG);

		val = response.indexOf('*');

		pkgTarget[0] = response.charAt(val+1);
		pkgTarget[1] = response.charAt(val+2);

        esp.end();

	    if (((pkgTarget[0] == 'r') || (pkgTarget[0] == 'b') || (pkgTarget[0] == 'g')) && ((pkgTarget[1] == 'r') || (pkgTarget[1] == 'b') || (pkgTarget[0] == 'g')))
	    {
	    	readStatus = 1;
	    }
    }
}

void serverWrite()
{
	esp.begin(9600);

	if(!esp.available()) // check if the esp is sending a message 
    {
    //Serial.println("Enter Message");
     //while(!Serial.available());

        //String text = Serial.readStringUntil("\n");

        String cipStart = "AT+CIPSTART=\"TCP\",\"192.168.1.2\",80\r\n";
        
        String params = "pkg1=";
        params += pkgColour[0];
        
        String post = "POST /SwarmServer.php HTTP/1.1\r\nHost: 192.168.1.2\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: ";
        post += params.length();
        post += "\r\n\r\n";
        post += params;
        post += "\r\n\r\n";
        
        String cipSend = "AT+CIPSEND=";
        cipSend += post.length();
        cipSend += "\r\n";
        
        sendData(cipStart,100,DEBUG);
        sendData(cipSend,100,DEBUG);        
        sendData(post,1000,DEBUG);

        cipStart = "AT+CIPSTART=\"TCP\",\"192.168.1.2\",80\r\n";
        
        params = "pkg2=";
        params += pkgColour[1];
        
        post = "POST /SwarmServer.php HTTP/1.1\r\nHost: 192.168.1.2\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: ";
        post += params.length();
        post += "\r\n\r\n";
        post += params;
        post += "\r\n\r\n";
        
        cipSend = "AT+CIPSEND=";
        cipSend += post.length();
        cipSend += "\r\n";
        
        sendData(cipStart,100,DEBUG);
        sendData(cipSend,100,DEBUG);        
        sendData(post,1000,DEBUG);

        cipStart = "AT+CIPSTART=\"TCP\",\"192.168.1.2\",80\r\n";
        
        params = "pkg3=";
        params += pkgColour[2];
        
        post = "POST /SwarmServer.php HTTP/1.1\r\nHost: 192.168.1.2\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: ";
        post += params.length();
        post += "\r\n\r\n";
        post += params;
        post += "\r\n\r\n";
        
        cipSend = "AT+CIPSEND=";
        cipSend += post.length();
        cipSend += "\r\n";
        
        sendData(cipStart,100,DEBUG);
        sendData(cipSend,100,DEBUG);        
        sendData(post,1000,DEBUG);

        cipStart = "AT+CIPSTART=\"TCP\",\"192.168.1.2\",80\r\n";
        
        params = "pkg4=";
        params += pkgColour[3];
        
        post = "POST /SwarmServer.php HTTP/1.1\r\nHost: 192.168.1.2\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: ";
        post += params.length();
        post += "\r\n\r\n";
        post += params;
        post += "\r\n\r\n";
        
        cipSend = "AT+CIPSEND=";
        cipSend += post.length();
        cipSend += "\r\n";
        
        sendData(cipStart,100,DEBUG);
        sendData(cipSend,100,DEBUG);        
        sendData(post,1000,DEBUG);

        cipStart = "AT+CIPSTART=\"TCP\",\"192.168.1.2\",80\r\n";
        
        params = "pkg5=";
        params += pkgTarget[0];
        
        post = "POST /SwarmServer.php HTTP/1.1\r\nHost: 192.168.1.2\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: ";
        post += params.length();
        post += "\r\n\r\n";
        post += params;
        post += "\r\n\r\n";
        
        cipSend = "AT+CIPSEND=";
        cipSend += post.length();
        cipSend += "\r\n";
        
        sendData(cipStart,100,DEBUG);
        sendData(cipSend,100,DEBUG);        
        sendData(post,1000,DEBUG);

        cipStart = "AT+CIPSTART=\"TCP\",\"192.168.1.2\",80\r\n";
        
        params = "pkg6=";
        params += pkgTarget[1];
        
        post = "POST /SwarmServer.php HTTP/1.1\r\nHost: 192.168.1.2\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: ";
        post += params.length();
        post += "\r\n\r\n";
        post += params;
        post += "\r\n\r\n";
        
        cipSend = "AT+CIPSEND=";
        cipSend += post.length();
        cipSend += "\r\n";
        
        sendData(cipStart,100,DEBUG);
        sendData(cipSend,100,DEBUG);        
        sendData(post,1000,DEBUG);

        esp.end();
	}
}

void drivePath()
{
    if (path[0] == '1')
    {
    	followRight(1);
    }

    else
    {
    	followLeft(1);
    }
        
    if (path[1] == '1')
    {
    	followRight(1);
    }

    else
    {
    	followLeft(1);
    }
        
    if (path[2] == '1')
    {
    	followRight(1);
    }

    else
    {
    	followLeft(1);
    }
        
    if (path[3] == '1')
    {
    	followRight(2);
    }

    else
    {
    	followLeft(1);
    }
}

void readPackages()
{
	followRight(1);
	followLeft(1);
	driveToStop();

	Move.Forward(10);
	delay(300);
	Move.Stop();
	delay(500);

	readColour();
	
	while(pkgColour[0] = 'u')
	{
		getColour(1);
		Move.TurnRight(15);
		delay(300);
		Move.Stop();
		delay(300);
		Move.TurnLeft(15);
		delay(300);
		Move.Stop();
	}
	

	delay(1000);

	Move.Reverse(10);
	delay(200);
	Move.Stop();

	turn180();
	followRight(1);
	followRight(1);
	driveToStop();

	Move.Forward(10);
	delay(300);
	Move.Stop();
	delay(500);
	
	readColour();

	getColour(2);

	delay(1000);

	Move.Reverse(10);
	delay(200);
	Move.Stop();


	turn180();
	followRight(1);
	followRight(1);
	driveToStop();

	Move.Forward(10);
	delay(300);
	Move.Stop();
	delay(500);

	readColour();

	getColour(3);
	delay(1000);

	Move.Reverse(10);
	delay(200);
	Move.Stop();

	turn180();
	followRight(1);
	followRight(1);
	driveToStop();

	Move.Forward(10);
	delay(300);
	Move.Stop();
	delay(500);
	
	readColour();
	getColour(4);
	
	delay(1000);

	Move.Reverse(10);
	delay(200);
	Move.Stop();

	turn180();
	followLeft(1);
	followRight(3);

	Move.Stop();
	delay(500);

	unsigned long timer = millis();

	while((millis() - timer) <= 3000)
	{
		followStraight();
	}

	Move.Stop();
	delay(200);
	turn180();

	Move.Stop();

	serverWrite();
}

void getPackages()
{
	while(readStatus != 1)
	{
		serverRead();
	}

	while(pkgCount < 1)	//GET FIRST PACKAGE
	{

		if(pkgTarget[0] == pkgColour[0])	//Check if target matches first slot
		{
			pkgColour[0] = 'u';

			driveToStop();
			//pick up package
			gripPackage();
			delay(1000);

			pkgCount++;

			turn180();
			followRight(1);
			followLeft(1);
			driveToStop();
			//drop package
			dropPackage();
			delay(1000);
			turn180();
			followRight(1);
			followRight(1);

			getReady();
		}

		if(pkgCount > 0)
		break;

		if(pkgTarget[0] == pkgColour[1])	//check if targget matches second slot
		{
			pkgColour[1] = 'u';

			followLeft(1);
			followRight(1);
			driveToStop();
			//pick up package
			gripPackage();
			delay(1000);

			pkgCount++;

			turn180();
			driveToStop();

			Move.Stop();
			delay(200);
			Move.Forward(20);
			delay(200);
			Move.Stop();
			delay(200);
			
			driveToStop();
			//drop package
			dropPackage();
			delay(1000);
			turn180();
			followRight(1);
			followRight(1);

			getReady();
		}

		if(pkgCount > 0)
		break;

		if(pkgTarget[0] == pkgColour[2])	//check if target matches third slot
		{
			pkgColour[2] = 'u';

			followLeft(1);
			followRight(2);
			driveToStop();
			//pick up package
			gripPackage();
			delay(1000);

			pkgCount++;

			turn180();
			followLeft(1);
			followRight(1);
			driveToStop();
			//drop package
			dropPackage();
			delay(1000);
			turn180();
			followRight(1);
			followRight(1);

			getReady();
		}

		if(pkgCount > 0)
		break;

		if(pkgTarget[0] == pkgColour[3])	//check if target matches fourth slot
		{
			pkgColour[3] = 'u';

			followLeft(1);
			followRight(3);
			driveToStop();
			//pick up package
			gripPackage();

			delay(1000);

			pkgCount++;

			turn180();
			followLeft(1);
			followRight(2);
			driveToStop();

			//drop package
			dropPackage();

			delay(1000);
			turn180();
			followRight(1);
			followRight(1);

			getReady();
		}

		break;
	}



	while(pkgCount < 2)	//GET SECOND PACKAGE	
	{

		if(pkgTarget[1] == pkgColour[0])	//Check if target matches first slot
		{
			pkgColour[0] = 'u';

			driveToStop();
			//pick up package
			gripPackage();
			delay(1000);

			pkgCount++;

			turn180();
			followRight(1);
			followLeft(2);
			driveToStop();
			//drop package
			dropPackage();
			delay(1000);
			turn180();
			followRight(1);
			followRight(2);

			getReady();
		}

		if(pkgCount > 1)
		break;

		if(pkgTarget[1] == pkgColour[1])	//check if targget matches second slot
		{
			pkgColour[1] = 'u';

			followLeft(1);
			followRight(1);
			driveToStop();
			//pick up package
			gripPackage();
			delay(1000);

			pkgCount++;

			turn180();
			followRight(1);
			followLeft(1);
			driveToStop();
			//drop package
			dropPackage();
			delay(1000);
			turn180();
			followRight(1);
			followRight(2);

			getReady();
		}

		if(pkgCount > 1)
		break;

		if(pkgTarget[1] == pkgColour[2])	//check if target matches third slot
		{
			pkgColour[2] = 'u';

			followLeft(1);
			followRight(2);
			driveToStop();
			//pick up package
			gripPackage();
			delay(1000);

			pkgCount++;

			turn180();
			driveToStop();
			
			Move.Stop();
			delay(200);
			Move.Forward(20);
			delay(200);
			Move.Stop();
			delay(200);

			driveToStop();
			//drop package
			dropPackage();
			delay(1000);
			turn180();
			followRight(1);
			followRight(2);

			getReady();
		}

		if(pkgCount > 1)
		break;

		if(pkgTarget[1] == pkgColour[3])	//check if target matches fourth slot
		{
			pkgColour[3] = 'u';

			followLeft(1);
			followRight(3);
			driveToStop();
			//pick up package
			gripPackage();
			delay(1000);

			pkgCount++;

			turn180();
			followLeft(1);
			followRight(1);
			driveToStop();

			//drop package
			dropPackage();

			delay(1000);
			turn180();
			followRight(1);
			followRight(2);

			getReady();
		}

		break;
	}

	serverWrite();

	//turn180();
	driveToStop();
	turn180();
	followRight(2);
	followLeft(1);
	driveToStop();
}

void initColor()
{
	if (tcs.begin())
    {
        Serial.println("Found sensor");
    }

    else
    {
        Serial.println("No TCS34725 found ... check your connections");
        while (1); // halt!
    }
}

void readColour()
{
	redSum = 0;
	greenSum = 0;
	blueSum = 0;

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

	red = redSum / 10;
	green = greenSum / 10;
	blue = blueSum / 10;

	Serial.print("Red: "); Serial.print(red);
	Serial.print("    Green: "); Serial.print(green);
	Serial.print("    Blue: "); Serial.println(blue);

    delay(100);
}

void getColour(int i)
{
	readColour();
	i--;

	if ((red > 0) && (red < 200) && (green > 200) && (green < 1000) && (blue > 150) && (blue < 500))
    {
       // Serial.println("Shit is green yo.");
       pkgColour[i] = 'g';
    }

    else if ((red > 30) && (red < 100) && (green > 30) && (green < 100) && (blue > 30) && (blue < 100))
    {
       // Serial.println("Shit is blue yo.");
       pkgColour[i] = 'b';
    }

    else if ((red > 200) && (red < 1200) && (green > 50) && (green < 350) && (blue > 50) && (blue < 350))
    {
       // Serial.println("Shit is red yo.");
       pkgColour[i] = 'r';
    }

    else
    {
       // Serial.println("idk what that shit is yo");
       pkgColour[i] = 'u';
    }
}

void debugColour()
{
	redSum = 0;
	greenSum = 0;
	blueSum = 0;

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

	red = redSum / 10;
	green = greenSum / 10;
	blue = blueSum / 10;

	Serial.print("Red: "); Serial.print(red);
	Serial.print("    Green: "); Serial.print(green);
	Serial.print("    Blue: "); Serial.println(blue);

	if ((red > 0) && (red < 200) && (green > 200) && (green < 1000) && (blue > 150) && (blue < 500))
    {
       Serial.println("Shit is green yo.");
       pkgColour[0] = 'g';
    }

    else if ((red > 30) && (red < 100) && (green > 30) && (green < 100) && (blue > 30) && (blue < 100))
    {
       Serial.println("Shit is blue yo.");
       pkgColour[0] = 'b';
    }

    else if ((red > 200) && (red < 1200) && (green > 50) && (green < 350) && (blue > 50) && (blue < 350))
    {
       Serial.println("Shit is red yo.");
       pkgColour[0] = 'r';
    }

    else
    {
       Serial.println("idk what that shit is yo");
       pkgColour[0] = 'u';
    }

    delay(100);
}

void debugServer()
{
	serverRead();

	Serial.println("PACKAGE TARGET");
	Serial.println("PACKAGE TARGET");

	Serial.println(pkgTarget[0]);
	Serial.println(pkgTarget[1]);

	pkgColour[0] = 'r';
	pkgColour[1] = 'b';
	pkgColour[2] = 'r';
	pkgColour[3] = 'b';


	serverWrite();

	Serial.println("PACKAGE TARGET");
	Serial.println("PACKAGE TARGET");

	Serial.println(pkgTarget[0]);
	Serial.println(pkgTarget[1]);

	serverRead();

	Serial.println("PACKAGE TARGET");
	Serial.println("PACKAGE TARGET");

	Serial.println(pkgTarget[0]);
	Serial.println(pkgTarget[1]);
}

void openGripper()
{
	gripper.write(85);
	delay(200);
}

void closeGripper()
{
	gripper.write(30);
	delay(200);
}

void gripPackage()
{
	Move.Forward(10);
	delay(300);
	Move.Stop();

	closeGripper();

	Move.Reverse(10);
	delay(200);
	Move.Stop();
}

void dropPackage()
{
	Move.Forward(10);
	delay(300);
	Move.Stop();

	openGripper();

	Move.Reverse(10);
	delay(200);
	Move.Stop();
}





