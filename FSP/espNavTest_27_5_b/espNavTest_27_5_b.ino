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

int leftCount = 0;
int rightCount = 0;

#define esp Serial2
#define DEBUG true
String currentDir;
char path[5];
int readStatus = 0;
int val = 0;

char pkgColour[5];
char pkgTarget[3];



void setup()
{
	Move.begin(52);
	Serial.begin(115200);
	Serial1.begin(9600);

	// sendData("AT+RST\r\n",5000,DEBUG); // reset module

	// while(readStatus == 0)
	// {
	// 	serverRead();
	// }

	// Serial.write(path[0]);
	// delay(7);
	// Serial.write(path[1]);
	// delay(7);
	// Serial.write(path[2]);
	// delay(7);
	// Serial.write(path[3]);
	// delay(7);

	// Serial.println("           PATH");

	// //delay(5000);

	 moveToLine();
	// drivePath();

	readPackages();
}

void loop()
{

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
			Move.TurnLeft(15);
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
			Move.TurnRight(15);
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

String sendData(String command, const int timeout, boolean debug)
{
    String response = "";
    
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

    val = response.indexOf('*');

	targetPkg[0] = response.charAt(val+1);
	targetPkg[1] = response.charAt(val+2);

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

        esp.end();

	    if (((targetPkg[0] == 'r') || (targetPkg[0] == 'b')) && ((targetPkg[1] == 'r') || (targetPkg[1] == 'b'))) 
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
        
        String params = "msg=";
        params += currentDir;
        
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
	//get colour 1

	delay(1000);

	turn180();
	followRight(1);
	followRight(1);
	driveToStop();
	//get colour 2

	delay(1000);

	turn180();
	followRight(1);
	followRight(1);
	driveToStop();
	//get colour 4

	delay(1000);

	turn180();
	followRight(1);
	followRight(1);
	driveToStop();
	//get colour 4
	

	delay(1000);

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
}

void getPkg()
{
	//Get First Package

	serverRead();

}