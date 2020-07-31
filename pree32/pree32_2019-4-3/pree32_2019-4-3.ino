#include <WiFi.h>
#include <HTTPClient.h>
#include <Preferences.h>
#include <stdio.h>  /* snprintf */
#include <math.h>

#define DEBUG true
#define sampleRate 1200
#define currentZero 0.2


//-----------------KRISTON ADDED-----------------------//

float voltageM_float, currentM_float, voltageC_float, currentC_float;	//variables to store calibration data
String productID, voltageM_string, currentM_string, voltageC_string, currentC_string;								//String to hold product ID
String inputString;								//String necessary for functions: inStringMatch, inString.

//Added three new functions
//String inString() - waits for user to enter a string via serial (after hitting enter), then returns that string.
//boolean inStringMatch(String msg) - checks if inputString matches the argument. Returns true if match else false.
//void serialCalibrate() - prompts the user to enter ProductID, and the set of calibration values and stores them in the variables above.  

//-----------------------------------------------------//


const int currentPin = 34;      // ACS712 pin to Arduino A4 port
const int voltagePin = 35;


float currentRMS = 0;
float voltageRMS = 115.0;
String currentRMSString;
String voltageRMSString;
/*sensor variables*/

char ssidAP[] = "PowerPree-";

const char passAP[] = "";
WiFiServer server(80);

String ssid;
String pass;

unsigned char AP = 0;
unsigned char pree = 1;
unsigned char preeTest = 5; 
unsigned char mode = AP;




Preferences preferences;

const byte sButt =  27; //12 //CBUTT //formerly c/nButton //
const byte sLED  =  12;//pree-2: 12 ;pree-1: 13; //27  //CLED //formerly c/nLED //old ver = 13 //new version 12
const byte cButt = 26; //14 //SBUTT //formerly s/wbutton
const byte cLED   = 14;  //26 //SLED  //formerly s/wLED //old ver = 14

volatile int cnIntCnt;
volatile int swIntCnt;

int cnNumInts;
int swNumInts;

unsigned char butTimer = 0;
unsigned long butStartTime;

unsigned long iniMillis;

int relayPin = 25;
int relayPinState = 0;
String relayPinString;
char * params[3] = {"","",""};
String params1 = "";
char post[300] = "anything";
char macAddArray[30] = "ab:12:34:56:78:cd";
String macAddString; //or pk
String uk;

uint8_t efuse_mac_int[6];
char efuse_mac_char[19];

String responseStringPree;

 char v[6] = "115.0"; //with null character
 char c[7] = "2.31";
 char r[3] = "-1";
 /*string stuff*/
int counter[sampleRate]; //prev 400
int currents[sampleRate]; //prev 200
int voltages[sampleRate]; //prev 200

int vMax;
int iMax;

int vMin = 1000;
int iMin = 1000;

int vMid;
int iMid;

int vDelta;
int iDelta;

int vpp;
int ipp;

unsigned long vTime;
unsigned long iTime;
unsigned long pTime1;
unsigned long pTime2;

float currentBuffer;
float voltageBuffer;
//int voltageRMS;

void setup() 
{
  Serial.begin(115200);
  Serial.println("yo");

  pinMode(relayPin, OUTPUT);
  pinMode(sLED, OUTPUT);
  pinMode(cLED, OUTPUT);
  pinMode(cButt, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(cButt), swHandle, FALLING);  
  pinMode(sButt, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(sButt), cnHandle, FALLING);
  bool TEST = false;

  digitalWrite(cLED, HIGH);
  
  delay(3000);

  digitalWrite(cLED, LOW);

	if(Serial.available())		//kriston edit 19/2/19
	{
	  ssid = Serial.readString();

	  if(ssid.equalsIgnoreCase("test\r\n"))
	    TEST = true;

	  ssid="";
	}


  

  
  while((digitalRead(cButt) == 0) && (digitalRead(sButt) == 0))
  {
    TEST = true;
    digitalWrite(sLED,HIGH);
    digitalWrite(cLED,HIGH);
  }

  digitalWrite(sLED,LOW);
  digitalWrite(cLED,LOW);
  
  esp_efuse_mac_get_default(efuse_mac_int);
  String str0; String str1; String str2;
  String str3; String str4; String str5; String str6;

  str0 = String(efuse_mac_int[0], HEX);
  str1 = String(efuse_mac_int[1], HEX);
  str2 = String(efuse_mac_int[2], HEX);
  str3 = String(efuse_mac_int[3], HEX);
  str4 = String(efuse_mac_int[4], HEX);
  str5 = String(efuse_mac_int[5], HEX);
  str6 = String(efuse_mac_int[6], HEX);

  macAddString += str0; macAddString += ":"; macAddString += str1; macAddString += ":"; macAddString += str2; 
  macAddString += ":"; macAddString += str3; macAddString += ":"; macAddString += str4; macAddString += ":"; macAddString += str5;
 
  
  Serial.println(macAddString);
  
  Serial.println("getting ssid from eeprom"); //getting ssid from eeprom
  preferences.begin("my-app", false);    //initiialise an "App"
  ssid = preferences.getString("ssid");   // get whatever is stored in that location
  pass = preferences.getString("pass");
  preferences.end();


  if(TEST == false)
  {
    Serial.println(ssid);
    if(ssid == 0)             //checking to see if an ssid was stored previously
    {  
      Serial.println("no ssid found"); // no ssid found
      mode = AP;
    }
    else //if ssid was set previously
    {
      mode = pree;
    }
  }

  if(TEST == true)
  {
    mode = preeTest;
  }



  preferences.begin("my-app", false);    //initiialise an "App"
  productID = preferences.getString("productID");
  voltageM_string = preferences.getString("voltageM");
  voltageC_string = preferences.getString("voltageC");
  currentM_string = preferences.getString("currentM");
  currentC_string = preferences.getString("currentC");
  preferences.end();

  voltageM_float = voltageM_string.toFloat();
  voltageC_float = voltageC_string.toFloat();
  currentM_float = currentM_string.toFloat();
  currentC_float = currentC_string.toFloat();

  strcat(ssidAP, productID.c_str());	//added by kriston
  Serial.print("SSID: ");
  Serial.println(ssidAP);

  Serial.print("Product ID: ");
  Serial.println(productID);  
  Serial.print("voltageM: ");
  Serial.println(voltageM_string);
  Serial.print("voltageC: ");
  Serial.println(voltageC_string);
  Serial.print("currentM: ");
  Serial.println(currentM_string);
  Serial.print("currentC: ");
  Serial.println(currentC_string);

}
//interrupt stuff
/*
//void swHandle()
//{
//  portENTER_CRITICAL_ISR(&swMux);
//  swIntCnt = 1;
//  portEXIT_CRITICAL_ISR(&swMux);
//}
//void cnHandle()
//{
//  portENTER_CRITICAL_ISR(&cnMux);
//  cnIntCnt = 1;
//  portEXIT_CRITICAL_ISR(&cnMux);
//}
//void cnIntCheck()
//{
//  if(cnIntCnt == 1)
//  {
//    portENTER_CRITICAL(&cnMux);
//    cnIntCnt = 0;
//    portEXIT_CRITICAL(&cnMux);
//    butTimer = 1;
//    butStartTime = millis();
//  }
//  if((digitalRead(sButt) == 0) && (butTimer == 1))
//  {
//    if(millis() > (butStartTime + 3000))
//    {
//      butTimer = 0;
//      mode = AP;      //enter AP mode
//    }
//  }
//  else
//  {
//    butTimer = 0;
//  }
//}
//void swIntCheck()
//{
//  if(swIntCnt == 1)
//  {
//    portENTER_CRITICAL(&swMux);
//    swIntCnt = 0;
//    portEXIT_CRITICAL(&swMux);
//    digitalWrite(relayPin, !digitalRead(relayPin) );//toggle
//    digitalWrite(cLED, digitalRead(relayPin) );
//    updateRelayPostStatus();
//  }
//}
*/

void cnPollCheck()
{
  
  if(digitalRead(cButt) == 0)
  {
    unsigned long buttStartTime = millis();
    while(digitalRead(cButt) == 0)
    {
      if(millis() > buttStartTime+3000)
        {
          if(mode != AP)
            mode = AP;
          else
          {
            mode = pree;
          }
          break;
        }
    }
  }
}

void swPollCheck()
{
   if(digitalRead(sButt) == 0)
   {
    unsigned long buttStartTime = millis();
   while(digitalRead(sButt) == 0)
     {
        if(millis() > buttStartTime+1500)
          break;
     }
     digitalWrite(relayPin, !digitalRead(relayPin) );//toggle
     digitalWrite(sLED, digitalRead(relayPin) );
     updateRelayPostStatus();
     while(digitalRead(sButt) == 0);
   }
    
}

void loop() 
{
 // cnIntCheck();  //checks to see if cn was pressed/held down
 // swIntCheck();  //checks to see if sw was pressed/held down
//  
  cnPollCheck();
  swPollCheck();
            
  if(mode == AP)
  {
    APMode();
  }
  if(mode == pree)
  {
    preeMode();
  }
  if(mode == preeTest)
  {
    preeTestMode();
  }
}

void APMode()
{

  digitalWrite(cLED, HIGH);
  delay(500);
  digitalWrite(cLED, LOW);
  delay(500);
  digitalWrite(cLED, HIGH);
  delay(500);
  digitalWrite(cLED, LOW);
  
  
  WiFi.mode(WIFI_STA);
  WiFi.softAP(ssidAP, passAP);

  
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
    
   server.begin();
    
  int value = 0;
  int check = 0;
  int stater = 0;
  String responseString = "";
  while(check == 0) //set condition for this
  {
    WiFiClient client = server.available();   // listen for incoming clients

    //////
    int s = 0;
      while(s < 2)
      {
        digitalWrite(cLED, HIGH);
        delay(300);
        digitalWrite(cLED, LOW);
        delay(300);
        s++;
      }
    /////
   // String responseString = "";
    if (client) 
    {                             // if you get a client,
      Serial.println("New Client.");           // print a message out the serial port

      client.println("Status: 200");                            //EDing this now
      client.println("Access-Control-Allow-Origin: *");
      client.println("Access-Control-Allow-Methods: GET,POST,DELETE");
   //   client.println("Access-Control-Allow-Headers: X-Requested-With, Phant-Private-Key");
      client.println("Content-Type: text/html");
      client.println("Connection: close");

      
      String currentLine = "";                // make a String to hold incoming data from the client
      while (client.connected()) 
      {            // loop while the client's connected
        /////
        int c = 0;
      if(stater == 0)
      { 
        while((c < 2))
        {
          digitalWrite(cLED, HIGH);
          delay(300);
          digitalWrite(cLED, LOW);
          delay(300);
          c++;
        }
        digitalWrite(cLED, HIGH);
        delay(500);
        digitalWrite(cLED, LOW);
      }
       ////
      
        if(digitalRead(cButt) == 0)
        {
          while(digitalRead(cButt) == 0);
          check = 2;
          break;      
        }
        
        if (client.available()) 
        {             // if there's bytes to read from the client,
          stater = 1;
          
          char c = client.read();             // read a byte, then
          responseString+=c;
          Serial.write(c);                    // print it out the serial monitor
          if(c == '~')
          {
            stater = 0;
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.println("got message from app");
            int i = 0;
            while( i < 3)
            {
              digitalWrite(cLED, HIGH);
              delay(500);
              digitalWrite(cLED, LOW);
              delay(500);
              i++;
            }
            check = 1;
            break;
          }
        }
       //   Serial.println("client connected. waiting on message.");
        if(!(client.connected()))
          break;
        
      }
         
      // close the connection:
      client.stop();
      Serial.println("Client Disconnected.");
       digitalWrite(cLED, HIGH);
        delay(500);
        digitalWrite(cLED, LOW);
        delay(500);
        digitalWrite(cLED, HIGH);
        delay(500);
        digitalWrite(cLED, LOW);
    }
 }

   if(check == 1)
   {
     //parse for ssid and pass
     int tokStart = responseString.indexOf("SSID");
     int tokEnd = responseString.indexOf("&");
     //String messageString = responseString.substring(tokStart+5, tokEnd);
     ssid = responseString.substring(tokStart+5, tokEnd);



     tokStart = responseString.indexOf("pass");
     //tokEnd = responseString.indexOf("&");
     tokEnd = responseString.indexOf("&u");
     pass = responseString.substring(tokStart+5, tokEnd);
     //pass = "";
     tokStart = responseString.indexOf("uk");
     tokEnd = responseString.indexOf("~");
     uk = responseString.substring(tokStart+3, tokEnd);

     //kriston added//

     if (ssid.equals("%"))
     {
     	ssid = "";
     	pass = "";
     }

     //------------//


     Serial.print("ssid=");
     Serial.print(ssid);
     Serial.println("~");
     Serial.print("pass=");
     Serial.print(pass);
     Serial.println("~");
     Serial.print("uk=");
     Serial.print(uk);
     Serial.println("~");
     
      //AP mode complete 
      //ssid = "MonaConnect";  //get from parsing
      //pass = "";             //get from parsing
      //AP mode complete 
  
      WiFi.mode(WIFI_STA);
  
      Serial.println("converting String to CharArray for WIFI.begin");
      char ssidChar[20];
      char passChar[20];
      ssid.toCharArray(ssidChar, 20);
      pass.toCharArray(passChar, 20);
   
   //attempt to connect
    Serial.println("Attempting to connect now");
    WiFi.begin(ssidChar,passChar);
    
    
    unsigned long timeout = 5000;
    unsigned long inimillis = millis();
    while ((WiFi.status() != WL_CONNECTED) && millis() < inimillis + timeout)
    { 
      ////
      int w = 0;
      while(w < 4)
      {
        digitalWrite(cLED, HIGH);
        delay(100);
        digitalWrite(cLED, LOW);
        delay(100);
        w++;
      }
      ////  
      Serial.println("trying...");
      delay(500);      
    }
    //if connection succesful
    if(WiFi.status() == WL_CONNECTED)
      {
        digitalWrite(cLED, HIGH);
        Serial.println("Connection succesful");
        Serial.println("storing to ssid&pass in EEPROM");
        //store ssid
        preferences.begin("my-app", false);
        preferences.putString("ssid", ssid);
        preferences.putString("pass", pass);
        preferences.end();

        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        Serial.println("MAC address: ");
        Serial.println(WiFi.macAddress());
        
        //initial post 
        Serial.println("making initial post to server");
        HTTPClient http;   
 
         // http.begin("http://posttestserver.com/post.php");  //Specify destination for HTTP request
         http.begin("http://www.powerpree.com/methods/postpree");
         //http.addHeader("Content-Type", "text/plain");             //Specify content-type header
         http.addHeader("Content-Type", "application/x-www-form-urlencoded");
         String inipost = "uk=";
         inipost += uk;
         inipost += "&pk=";
         inipost += macAddString;

         //KRISTON ADDED//

         inipost += "&sn=";
         inipost += productID;
         inipost += "&vm=";
         inipost += voltageM_string;         
         inipost += "&vc=";
         inipost += voltageC_string;
         inipost += "&im=";
         inipost += currentM_string;         
         inipost += "&ic=";
         inipost += currentC_string;

		inipost += "&ssid=";
		inipost += ssid;
         //-------------//

         Serial.print("inipost =");
         Serial.println(inipost);
         int httpResponseCode = http.POST(inipost);   //Send the actual POST request
       
         if(httpResponseCode>0)
         {
       
          String response = http.getString();                       //Get the response to the request
         
          Serial.println(httpResponseCode);   //Print return code
          Serial.println(response);           //Print request answer
       
         }else
         {
       
          Serial.print("Error on sending POST: ");
          Serial.println(httpResponseCode);
       
         }
       
         http.end();  //Free resources

        digitalWrite(cLED, HIGH);
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        Serial.println("MAC address: ");
        Serial.println(WiFi.macAddress());
        mode = pree;
      }
   
     else
     {
      Serial.println("Connection unsuccessful");
      ////
      int u = 0;
      while(u < 5)
      {
        digitalWrite(cLED, HIGH);
        delay(300);
        digitalWrite(cLED, LOW);
        delay(300);
        u++;
      }
      ////
      mode = AP;
      
     }
    digitalWrite(cLED, HIGH);
    //Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("MAC address: ");
    Serial.println(WiFi.macAddress());
   }
   if(check == 2)
   {
    
   }
    
}

void preeMode()
{
  //getVals2();
  getRawData();
  
  swPollCheck();
  updateRelayPostStatus();
  

  preePost();
  swPollCheck();
  
  
// switchRelay();
//  if(WiFi.status() != WL_CONNECTED && (millis() > iniMillis + 6000))//conn every 6 secs
//  {
//    //LED blink for no connection
//    iniMillis = millis();
//    connectUsingEEPROM();
//  }
//  else
//  {
//    //LED blink for connected
//    //make post req
//    //post
//    switchRelay();
//    
//  }
}

void preeTestMode()
{
	Serial.println("in test mode");
	int c = 0;
	int s = 0;
	int avgSize = 150;
	// int avgRaw[avgSize];
	// float avgCal[avgSize];
	int avgRaw = 0;
	float avgCal = 0;
	int i = 0;
	int exitl = 0;
	digitalWrite(sLED, HIGH);
	digitalWrite(cLED, HIGH);
	digitalWrite(relayPin, HIGH);
	delay(2000);
	digitalWrite(sLED, LOW);
	digitalWrite(cLED, LOW);
	digitalWrite(relayPin, LOW);
	delay(2000);

  	Serial.println("press cButt");	//kriston edit 19/2/19
    	digitalWrite(cLED,HIGH);
  	unsigned long startTim = millis();
  	while(digitalRead(cButt) == HIGH)// && (millis() < startTim + 5000));
    {
      if(Serial.available())
      {
        while(Serial.available())
          Serial.read();

        break;
      }
    }

  	Serial.println("pressed");
  	digitalWrite(sLED, HIGH);
  	digitalWrite(cLED, LOW);
  	Serial.println("press sButt");
  	startTim = millis();
  	while(digitalRead(sButt) == HIGH)// && (millis() < startTim + 5000));
    {
      if(Serial.available())
      {
        while(Serial.available())
          Serial.read();

        break;
      }
    }

	digitalWrite(relayPin, HIGH);
	int testModeMode = 0;

	while(1)
	{
    	digitalWrite(sLED, !digitalRead(sLED));		//toggle sLED for feedback on loop
 
		if(Serial.available())
		{
			inputString = inString();

			if(inStringMatch(""))
				testModeMode = 0;

			if(inStringMatch("vpp"))
				testModeMode = 1;

			if(inStringMatch("ipp"))
				testModeMode = 2;

			if(inStringMatch("ippraw"))
				testModeMode = 3;

			if(inStringMatch("vppraw"))
				testModeMode = 4;

			if(inStringMatch("bothraw"))
				testModeMode = 5;		

			if(inStringMatch("calibrate"))
				testModeMode = 6;

      if(inStringMatch("clear"))
        testModeMode = 7;  

			inputString = "";
		}

		if(testModeMode == 0) // regular data posts
		{
			getVals();
			calcVals();
			printVals();
		}

		if(testModeMode == 1)    //shows avg vpp and voltage reading
		{
			int avgRaw = 0;
			float avgCal = 0;
			int i = 0;

			while( i < avgSize )
			{
				getVals();
				calcVals();
				avgRaw += vpp;
				avgCal += voltageBuffer;
				i++;
			}

			avgRaw = avgRaw/avgSize;        //average vpp readings for size
			avgCal = avgCal/avgSize;       //average 
			Serial.print("avg vpp= ");
			Serial.print(avgRaw);
			Serial.print(", avg v= ");
			Serial.println(avgCal);
		}


		if(testModeMode == 2)    //shows avg vpp and voltage reading
		{
			int avgRaw = 0;
			float avgCal = 0;
			int i = 0;

			while( i < avgSize )
			{
				getVals();
				calcVals();
				avgRaw += ipp;
				avgCal += currentBuffer;
				i++;
			}

			avgRaw = avgRaw/avgSize;
			avgCal = avgCal/avgSize;       
			Serial.print("avg IPP= ");
			Serial.print(avgRaw);
			Serial.print(", avg C= ");
			Serial.println(avgCal);
		}


		if(testModeMode == 3)  //current graph
		{
			digitalWrite(sLED, LOW);
			digitalWrite(cLED, HIGH);
			printCurrents();
		}

		if(testModeMode == 4)  //voltage graph
		{
			digitalWrite(sLED, HIGH);
			digitalWrite(cLED, LOW);
			printVoltages();
		}

		if(testModeMode == 5)  //both graph
		{
			digitalWrite(sLED, HIGH);
			digitalWrite(cLED, HIGH);
			getVals();
			printBoth();
		}

		if(testModeMode == 6) //enter calibrated values
		{
			serialCalibrate();
			delay(5000);
			testModeMode = 0;  
		}

    if(testModeMode == 7) //enter calibrated values
    {
      ssid = "";
      pass = "";

      preferences.begin("my-app", false);
      preferences.putString("ssid", ssid);
      preferences.putString("pass", pass);
      preferences.end();

      Serial.println("SSID and Password Cleared");
      delay(5000);

      testModeMode = 0;  
    }
	}
}


//   while(exitl == 0)
//   {
   
//     if(digitalRead(cButt == LOW))
//     {
//       digitalWrite(cLED, LOW);
//       c = 1;
//     }
//     else
//     {
//       digitalWrite(cLED, HIGH);
//       c = 0;
//     } 
//     if(digitalRead(sButt == LOW))
//     {
//       digitalWrite(sLED, LOW);
//       s = 1;
//     }
//     else
//     {
//       digitalWrite(sLED, HIGH);
//       s = 0;
//     }   
//     if((s == 1) && (c == 1))
//       exitl = 1;
  
// }


//----------------KRISTON CALIBRATION CODE---------------------------//

String inString()    //Waits for the user to send command via Serial
{
  boolean serialComplete = false;
  String inString = "";

  while (serialComplete == false)
  {
    while (Serial.available()) 
    {
      char inChar = (char)Serial.read();
      inString += inChar;

      if (inChar == '\n') 
      {
        serialComplete = true;
      }
    }
  }

  return inString;
}

boolean inStringMatch(String msg)		//prepends argument string with NL and CR, and returns true if string match
{
	boolean matchBool = false;

	msg +='\r'; 
	msg +='\n';

	if(inputString.equalsIgnoreCase(msg))	//kriston edit 19/2/19
	matchBool = true;

	else
	matchBool = false;

	return matchBool;
}

void serialCalibrate()  //Prompts user to enter values then saves em'
{
  Serial.println("Enter Product ID");
  productID = inString();

  Serial.println("Enter Voltage Gradient(m)");
  voltageM_string = inString();
  voltageM_float = voltageM_string.toFloat();

  Serial.println("Enter Voltage intercept(C)");
  voltageC_string = inString();
  voltageC_float = voltageC_string.toFloat();

  Serial.println("Enter Current Gradient(m)");
  currentM_string = inString();
  currentM_float = currentM_string.toFloat();

  Serial.println("Enter Current Gradient(C)");
  currentC_string = inString();
  currentC_float = currentC_string.toFloat();

  Serial.print("Product ID: ");
  Serial.println(productID);  
  Serial.print("voltageM: ");
  Serial.println(voltageM_string);
  Serial.print("voltageC: ");
  Serial.println(voltageC_string);
  Serial.print("currentM: ");
  Serial.println(currentM_string);
  Serial.print("currentC: ");
  Serial.println(currentC_string);

  preferences.begin("my-app", false);
  preferences.putString("productID", productID);
  preferences.putString("voltageM", voltageM_string);
  preferences.putString("voltageC", voltageC_string);
  preferences.putString("currentM", currentM_string);
  preferences.putString("currentC", currentC_string);
  preferences.end();
}

//----------------------------------------------------------------//

void getVals()
{
  int count1 = 0;
  int avgCount = 0;
  vMax = 100;
  vMin = 3000;
  iMax = 100;
  iMin = 3000;
 // int avgSample[3];
  //while(avgCount < 4)
  //{
    while(count1 < sampleRate) //was <=
    {
    //  Serial.println("boom");
      //timeStamp[count1] = micros();
      voltages[count1] = analogRead(voltagePin);
      currents[count1] = analogRead(currentPin);

      
  
      if (vMax < voltages[count1])
      {
        vMax = voltages[count1];
        
        vTime = micros();
      }
      if (iMax < currents[count1])
      {
        iMax = currents[count1];
        iTime = micros();
      }
  
      if (vMin > voltages[count1])
      {
        vMin = voltages[count1];
      }
      if (iMin > currents[count1])
      {
        iMin = currents[count1];
      }

      pTime1 = vTime - iTime;
      pTime2 = iTime - vTime; 
      
      vMid = vMax + vMin;
      vMid = vMid/2;

      iMid = iMax +iMin;
      iMid  = iMid/2;
  
      vDelta = vMax - vMid;
      vpp = vMax - vMin;

      iDelta = iMax - iMid;
      ipp = iMax - iMin;
  
     // voltageRMS = ipp*1.14;
      count1++;
    }
   // avgSample[avgCount] = voltageRMS;
  //  avgCount++;
  //}
 // voltageRMS = (avgSample[0] + avgSample[1] + avgSample[2] + avgSample[3])/4;
  //voltages[count1] = 580;
}

void getVals2()
{
  int count1 = 0;
  int avgCount = 0;
  vMax = 100;
  vMin = 3000;
  iMax = 100;
  iMin = 3000;
 
    while(count1 <= sampleRate)
    {

      voltages[count1] = analogRead(voltagePin);
      currents[count1] = analogRead(currentPin);
  
      if (vMax < voltages[count1])
      {
        vMax = voltages[count1];
        
        vTime = micros();
      }
      if (iMax < currents[count1])
      {
        iMax = currents[count1];
        iTime = micros();
      }
  
      if (vMin > voltages[count1])
      {
        vMin = voltages[count1];
      }
      if (iMin > currents[count1])
      {
        iMin = currents[count1];
      }

//      pTime1 = vTime - iTime;
//      pTime2 = iTime - vTime; 
//      
//      vMid = vMax + vMin;
//      vMid = vMid/2;
//
//      iMid = iMax +iMin;
//      iMid  = iMid/2;
//  
//      vDelta = vMax - vMid;
//      vpp = vMax - vMin;
//
//      iDelta = iMax - iMid;
      ipp = iMax - iMin;
  
      count1++;
    }

  //}

}

void printVals()
{
  //Serial.print("vMax:");Serial.print(vMax);Serial.print(" vMin:");Serial.print(vMin);Serial.print("iMax:");Serial.print(iMax);Serial.print(" iMin:");Serial.print(vMin);
  
  Serial.print("ipp:");Serial.print(ipp); Serial.print(",current:");Serial.print(currentBuffer);Serial.print(",vpp:");Serial.print(vpp);Serial.print(",voltage:");Serial.print(voltageBuffer);
  Serial.print(",pTime1:");Serial.print(pTime1);Serial.print(",pTime2:");Serial.println(pTime2);
}

void connectUsingEEPROM()
{
    Serial.println("so yo i Pulling SSID and PASS from EEPROM");
    preferences.begin("my-app", false);
    ssid = preferences.getString("ssid");
    pass = preferences.getString("pass");
    preferences.end();
    Serial.println("this is what i got fam");
    Serial.println(ssid);
    Serial.println(pass);
    

    WiFi.mode(WIFI_STA);

    Serial.print("converting String to CharArray for WIFI.begin");
    char ssidChar[20];
    char passChar[20];
    ssid.toCharArray(ssidChar, 20);
    pass.toCharArray(passChar, 20);

    Serial.println("Attempting to connect now");
    WiFi.begin(ssidChar,passChar);
}

void preePost()
{
  if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
  digitalWrite(cLED, HIGH);
   HTTPClient http; 
   String post = "";
   post +=  "pk=";
   post += macAddString;
   post += "&v=";         // these
   post += voltageRMSString; // comment these out to disable voltage
   post += "&c=";        //these
   //post += "&v=115.7&c="; //"&v=115.7&c=15.99&r="; 
   post += currentRMSString;
   post += "&r=";
   post += relayPinString;
 
  // http.begin("http://posttestserver.com/post.php");  //Specify destination for HTTP request
    http.begin("http://www.powerpree.com/methods/postpree");
   //http.addHeader("Content-Type", "text/plain");             //Specify content-type header
   http.addHeader("Content-Type", "application/x-www-form-urlencoded");
   int httpResponseCode = http.POST(post);   //Send the actual POST request
 
   if(httpResponseCode>0)
   {
 
    String response = http.getString();                       //Get the response to the request
 
    Serial.println(httpResponseCode);   //Print return code
    Serial.println(response);           //Print request answer
    responseStringPree = response;
    int tokStart = responseStringPree.indexOf("toggle");
 
      String messageString = responseStringPree.substring(tokStart+7, tokStart+8);
      params1 = messageString;
    Serial.println(params1);
    switchRelay();
    
//    digitalWrite(cLED, HIGH);
//    delay(100);
//    digitalWrite(cLED, LOW);
    
    
   }else{
    digitalWrite(cLED, HIGH);
    delay(100);
    digitalWrite(cLED, LOW);
    delay(100);
    digitalWrite(cLED, HIGH);
    delay(100);
    digitalWrite(cLED, LOW);
   
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
 
   }
 
   http.end();  //Free resources
 
 }else{
    digitalWrite(cLED, LOW);
    Serial.println("Error in WiFi connection");
    connectAttempt();
       
 
 }
}

void connectAttempt()
{
  WiFi.mode(WIFI_STA);

    Serial.println("converting String to CharArray for WIFI.begin");
    char ssidChar[20];
    char passChar[20];
    ssid.toCharArray(ssidChar, 20);
    pass.toCharArray(passChar, 20);
   //attempt to connect
    Serial.println("Attempting to connect now");
    WiFi.begin(ssidChar,passChar);
    
    
    unsigned long timeout = 5000;
    unsigned long inimillis = millis();
    while ((WiFi.status() != WL_CONNECTED) && millis() < inimillis + timeout)
    {   
      ////
      int w = 0;
      while(w < 4)
      {
        digitalWrite(cLED, HIGH);
        delay(100);
        digitalWrite(cLED, LOW);
        delay(100);
        w++;
      }
      ////
      Serial.println("trying...");
      swPollCheck();
      delay(500);  
      swPollCheck();    
    }
    //if connection succesful
    if(WiFi.status() == WL_CONNECTED)
    {
      Serial.println("Connection succesful");
      Serial.println(macAddString);
      digitalWrite(cLED, HIGH);
    }
    else
    {
      Serial.println("Connection unsuccesful");
      digitalWrite(cLED, LOW);
    }
}

void getRawData()         //TO DO
{
//  float currentBuffer;
//  float voltageBuffer;
   //currentRMS =  roundf(readCurrent(currentPin)*100) / 100;
   //voltageRMS =  roundf(readVoltage(voltagePin)*100) / 100;
   getVals();
   if(DEBUG == true)
   {
    printVals();
   }
  //currentRMSString = String(ipp, DEC);
   //if(relayPinState == 1)
     calcVals();
//       currentBuffer = (ipp - 14.185)/218.04;
//  voltageBuffer = (vpp*0.1) - 149;
     if(DEBUG)
     {
      Serial.print(currentBuffer);
      Serial.println("A,");
      Serial.print(voltageBuffer);
      Serial.println("V");
     }
   if(digitalRead(relayPin) == 1)
   {
//    
//     currentBuffer = (ipp - 14.185)/218.04;
//     voltageBuffer = vpp;
//     if(DEBUG)
//     {
//      Serial.print(currentBuffer);
//      Serial.println("A,");
//      Serial.print(voltageBuffer);
//      Serial.println("V");
//     }
     currentRMSString = String(currentBuffer, 2);
     //currentRMSString = String(float(ipp), 2);
     voltageRMSString = String(voltageBuffer, 2);
//       currentRMSString = "0.2";
   }
   else
   {
    currentRMSString = "0.0";
    voltageRMSString = String(voltageBuffer, 2);
   }
}

void calcVals()
{
  currentBuffer = (ipp * currentM_float) + currentC_float;
  voltageBuffer = (vpp* voltageM_float) + voltageC_float;

  // currentBuffer = (ipp*0.00607) + (-0.791);
  // voltageBuffer = (vpp*0.104) + (-152);

  if(currentBuffer < currentZero)
    currentBuffer = 0;
}

void switchRelay()       //TO DO
{
  if ( params1 == "1" ) //??? //suh fool dawg?
        {
          //Serial.println("switch");
          
//          digitalWrite(relayPin, !digitalRead(relayPin) );
//          digitalWrite(cLED, digitalRead(cLED) );
         // if(relayPinState == 0)
          if(digitalRead(relayPin) == 0)
          {
            digitalWrite(relayPin, HIGH);
            digitalWrite(sLED, digitalRead(relayPin) );
            relayPinState = 1;
            Serial.println("switch on");
          }
          else
          {
            digitalWrite(relayPin, LOW);
            digitalWrite(sLED, digitalRead(relayPin) );
            relayPinState = 0;
            Serial.println("switch off");
          }
          //adc_zero_c = determineVQ(currentPin);   //NOT NEEDED MAYBE?
          
          //adc_zero_v = determineVQ(voltagePin);
          //params1 = "0";
        }
   else
   {
    Serial.println("no switch");
   }
}
void updateRelayPostStatus()
{
   //if(digitalRead(relayPin) == 0 )
   //if(relayPinState == 0)
   if(digitalRead(relayPin) == 0)
        {
//          r[0] = '-';
//          r[1] = '1';
          relayPinString = "-1";
            
        }
        else 
        {
          relayPinString = "1";
//          r[0] = '1';  
//          r[1] = '\0';
        }
}
//int determineVQ(int PIN) 
//{
//  Serial.print("estimating avg. quiscent voltage:");
//  long VQ = 0;
//  //read 5000 samples to stabilise value
//  for (int i=0; i<5000; i++) {
//    VQ += analogRead(PIN);
//    delay(1);//depends on sampling (on filter capacitor), can be 1/80000 (80kHz) max.
//  }
//  VQ /= 5000;
//  //Serial.print(map(VQ, 0, 1023, 0, 5000));Serial.println(" mV");
//  Serial.print(map(VQ, 0, 4096, 0, 3300));Serial.println(" mV");
//  return int(VQ);
//}

//float readCurrent(int PIN)
//{
//  //short adc_zero;  
//  unsigned long currentAcc = 0;
//  unsigned int count = 0;
//  unsigned long prevMicros = micros() - sampleInterval ;
//  while (count < numSamples)
//  {
//    if (micros() - prevMicros >= sampleInterval)
//    {
//      long adc_raw = analogRead(currentPin) - adc_zero_c;
//      currentAcc += (unsigned long)(adc_raw * adc_raw);
//      ++count;
//      prevMicros += sampleInterval;
//    }
//  }
//  float rms = sqrt((float)currentAcc/(float)numSamples) * (50 / 1024.0);   //'50' come from 5v/X=0.1v/1A of ACS712 0.1v per Amp spec on 20A version
//  //rms = rms * 64.8823529412;
//  Serial.print("rms returns: ");
//  Serial.println(rms);
//  return rms;
//  //Serial.println(rms);
//}

//float readVoltage(int PIN)
//{
//  
//  //short adc_zero;  
//  unsigned long currentAcc = 0;
//  unsigned int count = 0;
//  unsigned long prevMicros = micros() - sampleInterval ;
//  while (count < numSamples)
//  {
//    if (micros() - prevMicros >= sampleInterval)
//    {
//      long adc_raw = analogRead(voltagePin) - adc_zero_v;
//      currentAcc += (unsigned long)(adc_raw * adc_raw);
//      ++count;
//      prevMicros += sampleInterval;
//    }
//  }
//  float rms = sqrt((float)currentAcc/(float)numSamples) * (50 / 1024.0);   //'50' come from 5v/X=0.1v/1A of ACS712 0.1v per Amp spec on 20A version
//  rms = rms * 64.8823529412;
//  return rms;
//  //Serial.println(rms);
//}

void printCurrents()
{
    int count1 = 0;
  int buff;
  int state = 0;
//  Serial.print("570");
//  Serial.print(",");
  while(count1 <= sampleRate)//80 before
  {
    if(state == 0)
      state = 1;
    else
 //     Serial.print("440");
  //    Serial.print(",");
   // Serial.println("bang");
    buff = currents[count1];
   
    //Serial.print(currents[count1]);
   // Serial.print(buff);
    Serial.println(buff);
//    Serial.print(",");   ///recomment for voltage
//    buff = voltages[count1]; ///recomment for voltage
//    Serial.println(buff); ///recomment for voltage
    
   // Serial.println("4");
    //Serial.println(voltages[count1]);
    
    count1++;
  }
}
void printVoltages()
{
   int count1 = 0;
  int buff;
  int state = 0;
//  Serial.print("570");
//  Serial.print(",");
  while(count1 <= sampleRate)//80 before
  {
    if(state == 0)
      state = 1;
    else
    {
   //     Serial.print("440");
   //    Serial.print(",");
   // Serial.println("bang");
//    buff = currents[count1];
   
    //Serial.print(currents[count1]);
   // Serial.print(buff);
 //   Serial.println(buff);
//    Serial.print(",");   ///recomment for voltage
    buff = voltages[count1]; ///recomment for voltage
   Serial.println(buff); ///recomment for voltage
    
   // Serial.println("4");
    //Serial.println(voltages[count1]);
    
    count1++;
    }
  }
}
void printBoth()
{
  int count1 = 0;
  int buff;
  int state = 0;
//  Serial.print("570");
//  Serial.print(",");
  while(count1 <= sampleRate)//80 before
  {
    if(state == 0)
      state = 1;
    else
 //     Serial.print("440");
  //    Serial.print(",");
   // Serial.println("bang");
    buff = currents[count1];
   
    //Serial.print(currents[count1]);
   // Serial.print(buff);
    Serial.print(buff);
    Serial.print(",");   ///recomment for voltage
    buff = voltages[count1]; ///recomment for voltage
    Serial.println(buff); ///recomment for voltage
    
   // Serial.println("4");
    //Serial.println(voltages[count1]);
    
    count1++;
  }
}


void getFreq()  //uses 
{
  int state = 0;
  unsigned long endTime;
  unsigned long startTime;
  while(state != 10)
  {
    int rVoltage = analogRead(voltagePin);
    if(rVoltage == 0)              //looks for the first 0 to cancel first incomplete pulse
    {
     //startTime = micros();
     state = 1;
    }
    while(state == 1)             //looks for rising edge of first pulse
    {
      rVoltage = analogRead(voltagePin);
      if(rVoltage > 5)     
      state = 2;
    }
     while( state == 2)          //looks for first 0 after pulse to start clock
     {
        rVoltage = analogRead(voltagePin);
        if(rVoltage == 0)
        {
          startTime = micros();
          state = 3;    
        }
     }
     while(state == 3)         //looks for first rising edge after pulse to be measured
      {
        rVoltage = analogRead(voltagePin);
        if(rVoltage > 5)
          {
            //endTime = micros();
            state = 4;         
          }
      }
      while(state == 4)       //looks for second 0 to take period
      {
        rVoltage = analogRead(voltagePin);
        if(rVoltage == 0)
          {
            endTime = micros();
            state = 5;         
          }
      }
      while(state == 5)
      {
       float period = endTime - startTime;
       float freq = 1/period;
       freq = freq*1000000;
       Serial.print("period: ");
       Serial.println(period);
       Serial.print("freq: ");
       Serial.println(freq);
       state = 10;
      }
    }
}
