#include <SoftwareSerial.h>

#define DEBUG true
#define HOST_IP 169.254.182.104

SoftwareSerial esp(2,3); // make RX Arduino line is pin 2, make TX Arduino line is pin 3.
                             // This means that you need to connect the TX line from the esp to the Arduino's pin 2
                             // and the RX line from the esp to the Arduino's pin 3
String r = "";
void setup()
{
    Serial.begin(9600);
    Serial.println("Initiallized");
    esp.begin(9600); // your esp's baud rate might be different
    
    sendData("AT+RST\r\n",6000,DEBUG); // reset module
}

void loop()
{
    if(!esp.available()) // check if the esp is sending a message 
    {
        Serial.println("Enter message");
        
        while(!Serial.available());

        String text = Serial.readStringUntil("\n");

        String cipStart = "AT+CIPSTART=\"TCP\",\"192.168.4.142\",1025\r\n";
        
        String params = "id=1234&pot=F00&but=2";
        //params += text;
       
        String post = "POST /browserPost HTTP/1.1\r\nHost: 192.168.4.142:1025\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: ";
        post += params.length();
        post += "\r\n\r\n";
        post += params;
        post += "\r\n\r\n";
        
        String cipSend = "AT+CIPSEND=";
        cipSend += post.length();
        cipSend += "\r\n";
        
        sendData(cipStart,2000,DEBUG);

        sendData(cipSend,2000,DEBUG);        
        sendData(post,2000,DEBUG);
    }
}
 
 
String sendData(String command, const int timeout, boolean debug)
{
    String response = "";
    
    esp.print(command); // send the read character to the esp
    
    long int time = millis();
    int check = 0;
    
    while(((time+timeout) > millis()) || check == 0)
    {
        while(esp.available())
        {
            Serial.write(esp.read());
        }
        
        check  = 1;  
    }
    
    if(debug)
    {
       	while(esp.available())
        {
            Serial.write(esp.read());
        }  
    }
}