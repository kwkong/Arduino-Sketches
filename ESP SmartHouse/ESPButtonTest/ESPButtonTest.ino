#include <SoftwareSerial.h>

#define DEBUG true

SoftwareSerial esp(3,2); // make RX Arduino line is pin 2, make TX Arduino line is pin 3.
                             // This means that you need to connect the TX line from the esp to the Arduino's pin 2
                             // and the RX line from the esp to the Arduino's pin 3
String r = "";
int val;

void setup()
{
    Serial.begin(9600);
    esp.begin(9600); // your esp's baud rate might be different
    
    sendData("AT+RST\r\n",5000,DEBUG); // reset module

    pinMode(13, OUTPUT);
}

void loop()
{
    
    if(!esp.available()) // check if the esp is sending a message 
    {

        String cipStart = "AT+CIPSTART=\"TCP\",\"172.16.193.69\",80\r\n";
        
        String params = "esp=";
        
        String post = "POST /ESPButtonTest.php HTTP/1.1\r\nHost: 172.16.193.69\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: ";
        post += params.length();
        post += "\r\n\r\n";
        post += params;
        post += "\r\n\r\n";
        
        String cipSend = "AT+CIPSEND=";
        cipSend += post.length();
        cipSend += "\r\n";
        
        sendData(cipStart,500,DEBUG);

        sendData(cipSend,500,DEBUG);        
        sendData(post,500,DEBUG);

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

        val = response.indexOf('*');

        if (val > 0)
     	{
			if ((response.charAt(val) == '*') && (response.charAt(val+1) == 'o') && (response.charAt(val+2) == 'n'))
			{
				digitalWrite(13, HIGH);
	        }

	        if ((response.charAt(val) == '*') && (response.charAt(val+1) == 'o') && (response.charAt(val+2) == 'f') && (response.charAt(val+3) == 'f'))
			{
				digitalWrite(13, LOW);
	        }

    	}
    
    	return response;
	}
}