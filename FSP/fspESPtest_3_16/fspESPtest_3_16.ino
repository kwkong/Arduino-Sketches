#include <PWMServo.h>

#include <SoftwareSerial.h>

#include <Adafruit_Colour_Sensor.h>

#include <Wire.h>




Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

PWMServo claw;

#define DEBUG true

const int servoPin = 9;

String sendColour;
String r = "";

int openAngle = 65;
int closeAngle = 45;

int val;

SoftwareSerial esp(2,3); // make RX Arduino line is pin 2, make TX Arduino line is pin 3.
                             // This means that you need to connect the TX line from the esp to the Arduino's pin 2
                             // and the RX line from the esp to the Arduino's pin 3

void setup()
{
	claw.attach(9);
    Serial.begin(9600);
     // your esp's baud rate might be different
    
    sendData("AT+RST\r\n",5000,DEBUG); // reset module
}

void loop()
{
    getColour();
	serverRead();
	serverWrite();

	gripper();

}

void getColour()
{
    uint16_t red, green, blue, clear;
    uint16_t reds[10], greens[10], blues[10];
    uint16_t redSum, greenSum, blueSum;

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

	//tcs.getRawData(& red, & green, & blue, &clear);

	//Serial.print("Red: "); Serial.print(red);
	//Serial.print("    Green: "); Serial.print(green);
	//Serial.print("    Blue: "); Serial.println(blue);



    if ((red > 0) && (red < 200) && (green > 300) && (green < 1000) && (blue > 200) && (blue < 500))
    {
        //Serial.println("Shit is green yo.");
        sendColour = "Green";

    }

    else if ((red > 0) && (red < 200) && (green > 100) && (green < 300) && (blue > 300) && (blue < 800))
    {
        //Serial.println("Shit is blue yo.");
        sendColour = "Blue";
    }

    else if ((red > 400) && (red < 1000) && (green > 100) && (green < 300) && (blue > 100) && (blue < 300))
    {
        //Serial.println("Shit is red yo.");
        sendColour = "Red";
    }

    else
    {
        //Serial.println("idk what that shit is yo");
        sendColour = "Unknown";
    }

    //Serial.println(sendColour);

 //   delay(300);
}

void gripper()
{

	val = sendColour.indexOf('U');

	if (sendColour.charAt((val+1)) == 'n')
	{
		claw.write(openAngle);
	}

	else
	{
		claw.write(closeAngle);
	}
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
    
    if(debug)
    {
        Serial.print(response);
    }
    
    return response;
}


void serverRead()
{
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
        params += sendColour;
        
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
 
