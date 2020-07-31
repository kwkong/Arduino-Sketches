#include <SoftwareSerial.h>

#define HOST_IP "192.168.4.147"
#define HOST_PORT "1025"
#define idNumber "620081275"

#define potPin A7
#define butPin 4

#define espRX 3
#define espTX 2

SoftwareSerial esp(espRX, espTX);

int pot = 0;
int rVal = 0;
int gVal = 0;
int bVal = 0;
int butCount = 1;
char butChar = '1';
String potString = "";
String params = "";
String paramsOld = "";


void setup()
{
	pinMode(potPin, INPUT);
	pinMode(butPin, INPUT_PULLUP);

    Serial.begin(9600);
    Serial.println("Initiallized");

    espInit();

    delay(1000);
}

void loop()
{
	Serial.println(params);

	paramsGet();
	butGet();
	potGet();
	espUpdate();
}
 
 
String espSend(String command)
{
    String response = "";
    
    esp.print(command); // send the read character to the esp

    while(!esp.available());

    while(esp.available())
    {
    	response += (esp.readString());
    	delay(20);
    }

    Serial.println(response);
    return response;
}

void espUpdate()
{
	while(esp.available())
	{
		Serial.write(esp.read());
		delay(50);
	}

	if(!paramsOld.equals(params))
	{
		Serial.println("New Params");
		String cipStart = "AT+CIPSTART=\"TCP\",\"";
        cipStart+= HOST_IP;
        cipStart+= "\",";
        cipStart+= HOST_PORT;
        cipStart+="\r\n";
        
        String post = "POST /browserPost HTTP/1.1\r\nHost: ";
        post += HOST_IP;
        post += "\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: ";

        post += params.length();
        post += "\r\n\r\n";
        post += params;
        post += "\r\n\r\n";
        
        String cipSend = "AT+CIPSEND=";
        cipSend += post.length();
        cipSend += "\r\n";
        
        espSend(cipStart);
        espSend(cipSend);        
        espSend(post);

        paramsOld = params;
	}
}

void espInit()
{
    esp.begin(9600); 

    while(esp.available())
    {
    	esp.read();
    	delay(50);
    }

    espSend("AT+RST\r\n"); // reset module
    Serial.println("ESP Ready");
}

char numToHex(int val)
{
	char hexOut = '0';

	val = map(val, 0, 9, 0, 15);

	switch (val) 
	{
	    case 0 : 
	      hexOut = '0';
	      break;

	    case 1 : 
	      hexOut = '1';
	      break;

	    case 2 : 
	      hexOut = '2';
	      break;

	    case 3 : 
	      hexOut = '3';
	      break;

	    case 4 : 
	      hexOut = '4';
	      break;

	    case 5 : 
	      hexOut = '5';
	      break;

	    case 6 : 
	      hexOut = '6';
	      break;

	    case 7 : 
	      hexOut = '7';
	      break;

	    case 8 : 
	      hexOut = '8';
	      break;

	    case 9 : 
	      hexOut = '9';
	      break;

	    case 10 : 
	      hexOut = 'A';
	      break;

	    case 11 : 
	      hexOut = 'B';
	      break;

	    case 12 : 
	      hexOut = 'C';
	      break;

	    case 13 : 
	      hexOut = 'D';
	      break;

	    case 14 : 
	      hexOut = 'E';
	      break;

	    case 15 : 
	      hexOut = 'F';
	      break;

	    default:
	    break;
	      // do something
	}

	return hexOut;
}

void potGet()
{
	pot = analogRead(potPin);
	pot = map(pot, 0, 980, 0, 999);	//1 2 3

	rVal = pot/100;
	gVal = pot/10; gVal = gVal%10;
	bVal = pot%100; bVal = bVal%10;

	potString = numToHex(rVal);
	potString += numToHex(gVal);
	potString += numToHex(bVal);
}

void butGet()
{
	if(!digitalRead(butPin))
	{
		while(!digitalRead(butPin));
		butCount++;

		if(butCount > 3)
			butCount = 1;
	}

	switch (butCount) 
	{
	    case 1:
	      butChar = '1';
	      break;

	    case 2:
	      butChar = '2';
	      break;

	    case 3:
	      butChar = '3';
	      break;

	    default:
	      break;
	}
}

void paramsGet()
{
    params += "id=";
    params += idNumber;
    params += "&but=";
    params += butChar;
    params += "&pot=";
    params += potString;
}