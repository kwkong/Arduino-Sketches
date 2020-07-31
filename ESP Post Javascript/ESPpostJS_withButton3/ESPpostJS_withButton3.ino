#include <SoftwareSerial.h>

#define HOST_IP "192.168.4.123"
#define HOST_PORT "1026"
#define idNumber "620081275"

#define debugLedPin 13
#define potPin A7
#define buzzPin 5
#define ledPin A6
#define butPin A4

#define espRX 3
#define espTX 2

SoftwareSerial esp(espRX, espTX);

int pot = 0;
int rVal = 0;
int gVal = 0;
int bVal = 0;
int freq = 0;
int ledState = 0;
int butCount = 1;
int err = 0;
char butChar = '1';
String potString = "";
String params = "";
String paramsOld = "";
String espCommandString = "";
String post = "";
String response = "";

int length = 0;


void setup()
{
	pinMode(potPin, INPUT);
	pinMode(butPin, INPUT_PULLUP);
	pinMode(buzzPin, OUTPUT);
	pinMode(ledPin, OUTPUT);
	pinMode(debugLedPin, OUTPUT);

 	//espCommandString.reserve(200);
	response.reserve(200);

    Serial.begin(9600);
    Serial.println("Initiallized");

    espInit();

    delay(1000);

    Serial.println("Start Connection?");

    while(!Serial.available())
    {
    	if(esp.available())
    	{
    		Serial.write(esp.read());
    	}
    }

    Serial.println(Serial.readString());
}

void loop()
{
	while(Serial.available())
		Serial.write(Serial.read());

	while(esp.available())
		Serial.write(esp.read());

	paramsGet();
	potGet();
	butGet();

}
 
 
String espSend(String command)
{
    //response = "";
   	char responseChar = 0;

    String parseString = "";
    int dataLength = 0;
    
    esp.print(command); // send the read character to the esp

     //delay(2000);

     unsigned long time = millis();

    // while(!esp.available())
    // {
    // 	if ((millis() - time) > 5000);
    // 	{
    // 		err = 1;
    // 		break;
    // 	}
    // }

    // while(esp.available())
    // {
    // 	responseChar = (esp.read());
    // 	response += responseChar;
    // 	//delay(100);
    // }

    while((millis() - time) < 200)
    {
	    if(esp.available())
	    {
	    	responseChar = (esp.read());
	    	response += responseChar;
	    	//delay(100);
	    }
    }

    //Serial.println(response);

    //Serial.println(response);

    // delay(2000);

    // Serial.println(response);

    if((response.charAt(response.indexOf('E')+1) == 'R')&&(response.charAt(response.indexOf('E')+2) == 'R'))
    {
    	Serial.println("das an error");
    	err = 1;
    }

    // if((response.charAt(response.indexOf('*')+1) == 'I'))
    // {
    // 	dataLength = response.indexOf('%') - response.indexOf('$');

    // 	for(int i=1; i<(dataLength+1); i++)
    // 	{	
    // 		parseString += response.charAt(response.indexOf('$')+i);
    // 	}

    // 	Serial.print("parseString: ");
    // 	Serial.println(parseString);

    // 	freq = parseString.toInt();
    // }



    return response;
}

void espUpdate()
{
	if(!paramsOld.equals(params))
	{
		Serial.println("New Params");

		espCommandString ="";

		espCommandString = "AT+CIPSTART=\"TCP\",\"";
		espCommandString+= HOST_IP;
		espCommandString+= "\",";
		espCommandString+= HOST_PORT;
		espCommandString+="\r\n";

		espSend(espCommandString);

        post = "POST /espPost HTTP/1.1\r\nHost: ";
        post += HOST_IP;
        post += "\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: ";
        post += params.length();
        post += "\r\n\r\n";
        post += params;
        post += "\r\n\r\n";

        espCommandString ="";
        espCommandString = "AT+CIPSEND=";
        espCommandString += post.length();
        espCommandString += "\r\n";

        espSend(espCommandString);
        espSend(post);
        espSend("AT+CIPCLOSE\r\n");

        paramsOld = params;
	}
}

void espInit()
{
    esp.begin(9600); 
    //esp.setTimeout(500);

    while(esp.available())
    {
    	esp.read();
    	delay(50);
    }

    //espSend("AT+RST\r\n"); // reset module

    esp.write("AT+RST\r\n");

    while(!esp.available());

    while(esp.available())
    {
    	Serial.print(esp.readString());
    }
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

		response  = "";
		espUpdate();
		Serial.println(response);
		Serial.print("Freq: ");
		Serial.println(freq);
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
    params = "id=";
    params += idNumber;
    params += "&but=";
    params += butChar;
    params += "&pot=";
    params += potString;
}