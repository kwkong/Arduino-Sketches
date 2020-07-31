#include <SoftwareSerial.h>

#define HOST_IP "172.16.193.106"		//IP address of HOST server 
#define HOST_PORT "1026"			//Port for HOST server
#define idNumber "620081275"		//Your ID number

#define debugLedPin 13
#define potPin A7
#define buzzPin 5
#define ledPin A5
#define butPin A4

#define espRX 3
#define espTX 2
#define espTimeout_ms 400	//increase this value to give the code more time to read the response from the ESP

SoftwareSerial esp(espRX, espTX);

int pot = 0;
int rVal = 0;
int gVal = 0;
int bVal = 0;
int freq = 0;
int ledState = 0;
int butCount = 1;
int butCountOld = 1;
int potOld = 1000;
int err = 0;
char butChar = '1';
String potString = "";
String params = "";
String paramsOld = "";
String espCommandString = "";
String post = "";
String response = "";


void setup()
{
	pinMode(potPin, INPUT);
	pinMode(butPin, INPUT_PULLUP);
	pinMode(buzzPin, OUTPUT);
	pinMode(ledPin, OUTPUT);
	pinMode(debugLedPin, OUTPUT);

	//response.reserve(200);			//reserves some memory for the response string

    Serial.begin(9600);
    Serial.println("Initiallized");

    espInit(9600);

    delay(3000);	//gives ESP some time to get IP
}

void loop()
{
	potGet();
	butGet();	
	paramsGet();
	espUpdate();
	parseResponse();
	
}
 
void espSend(String command)
{
   	char responseChar = 0;

    esp.print(command); // send the read character to the esp

     unsigned long time = millis();


    while((millis() - time) < espTimeout_ms)
    {
	    while(esp.available())
	    {
	    	responseChar = (esp.read());
	    	response += responseChar;
	    }
    }
}

void espUpdate()
{
	while(esp.available())
	{
		Serial.write(esp.read());
	}

	if(!paramsOld.equals(params))	//only updates if one of the parameters has changed
	{
		digitalWrite(13, HIGH);

		paramsOld = params;

		response  = "";
		Serial.println("New Params");
		delay(100);

		espCommandString ="";

		espCommandString = "AT+CIPSTART=\"TCP\",\"";
		espCommandString+= HOST_IP;
		espCommandString+= "\",";
		espCommandString+= HOST_PORT;
		espCommandString+="\r\n";

		espSend(espCommandString);		//starts the connection to the server
		delay(100);

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

        espSend(espCommandString);		//sends post length
        delay(100);

        espSend(post);			//sends POST request with the parameters 
        delay(100);

        espSend("AT+CIPCLOSE\r\n");		//closes server connection
        delay(100);	

        Serial.println(response);
        digitalWrite(13, LOW);
	}
}

void espInit(int espBaud)
{
    esp.begin(espBaud); 

    while(esp.available())
    {
    	esp.read();
    	delay(50);
    }

    esp.write("AT+RST\r\n");

    while(!esp.available());

    while(esp.available())
    {
    	Serial.print(esp.readString());
    }
}

char numToHex(int val)		//converts a decimal integer to hex char
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

	if (( pot < (potOld-10)) || (pot > (potOld+10)))	//only changes the value if the pot value changes significantly
	{
		potOld = pot;
		pot = map(pot, 0, 1023, 0, 999);	//maps the pot analog range to 999 for hex RGB conversion 


		rVal = pot/100;
		gVal = pot/10; gVal = gVal%10;
		bVal = pot%100; bVal = bVal%10;		//gets each individual digit from pot value

		potString = numToHex(rVal);
		potString += numToHex(gVal);
		potString += numToHex(bVal);
	}

}

void butGet()
{
	if(!digitalRead(butPin))
	{
		while(!digitalRead(butPin));
		butCount++;

		if(butCount > 3)
			butCount = 1;

		butCountOld = butCount;
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

void parseResponse()
{
	String parseString = "";
	int dataLength = 0;

    if((response.charAt(response.indexOf('E')+1) == 'R')&&(response.charAt(response.indexOf('E')+2) == 'R'))
    {
    	//Serial.println("Error Detected");
    	err = 1;
    }

    if((response.charAt(response.indexOf('*')+1) == 'I'))
    {
    	dataLength = response.indexOf('%') - response.indexOf('$');

    	dataLength -= 1;

    	for(int i=1; i<(dataLength+1); i++)
    	{	
    		parseString += response.charAt(response.indexOf('$')+i);
    	}

    	freq = parseString.toInt();

    	analogWrite(buzzPin, freq);

    	dataLength = 0;
    	parseString = "";

    	dataLength = response.indexOf('&') - response.indexOf('^');

    	dataLength -= 1;

    	for(int i=1; i<(dataLength+1); i++)
    	{	
    		parseString += response.charAt(response.indexOf('^')+i);
    	}


    	if (parseString.equals("true"))
    		digitalWrite(ledPin, HIGH);


    	if (parseString.equals("false"))
			digitalWrite(ledPin, LOW);
    }
}