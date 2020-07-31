#include <SoftwareSerial.h>

#define HOST_IP "192.168.4.147"
#define HOST_PORT "1025"

SoftwareSerial esp(3,2); // make RX Arduino line is pin 2, make TX Arduino line is pin 3.
                             // This means that you need to connect the TX line from the esp to the Arduino's pin 2
                             // and the RX line from the esp to the Arduino's pin 3
void setup()
{
    Serial.begin(9600);
    Serial.println("Initiallized");
    esp.begin(9600); // your esp's baud rate might be different
    esp.flush();
    sendData("AT+RST\r\n",6000); // reset module
}

void loop()
{
    while(esp.available()) // check if the esp is sending a message 
    {
        Serial.println("Enter message");
        
        while(!Serial.available());

        String text = Serial.readStringUntil("\n");

        String cipStart = "AT+CIPSTART=\"TCP\",\"";
        cipStart+= HOST_IP;
        cipStart+= "\",";
        cipStart+= HOST_PORT;
        cipStart+="\r\n";
        
        String post = "POST /browserPost HTTP/1.1\r\nHost: ";
        post += HOST_IP;
        post += "\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: ";

        String params = "";
        params += text;

        post += params.length();
        post += "\r\n\r\n";
        post += params;
        post += "\r\n\r\n";
        
        String cipSend = "AT+CIPSEND=";
        cipSend += post.length();
        cipSend += "\r\n";
        
        sendData(cipStart,2000);
        sendData(cipSend,2000);        
        sendData(post,2000);
    }
}
 
 
void sendData(String command, const int timeout)
{
    String response = "";
    
    esp.print(command); // send the read character to the esp

    while(!esp.available());

    while(esp.available())
    {
    	response += (esp.readString());
    	delay(10);
    }

    Serial.println(response);
    response = "";
}