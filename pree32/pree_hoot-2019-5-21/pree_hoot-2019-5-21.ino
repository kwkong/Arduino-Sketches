/*
 * Revised 3/9/18
 *  Added toggle check to connect Attempt loop
*/
#include <WiFi.h>
#include <HTTPClient.h>

#include <Preferences.h>

#define DEBUG true
//#define TEST true //change to true to enter testpree mode
#include <stdio.h>  /* snprintf */

//#include <MemoryFree.h>
#include <math.h>

#define sampleRate 1200

#define currentZero 0.2


//-----------------KRISTON ADDED-----------------------//

float voltageM_float, currentM_float, voltageC_float, currentC_float;  //variables to store calibration data
String productID, voltageM_string, currentM_string, voltageC_string, currentC_string;               //String to hold product ID
String inputString;               //String necessary for functions: inStringMatch, inString.

//Added three new functions
//String inString() - waits for user to enter a string via serial (after hitting enter), then returns that string.
//boolean inStringMatch(String msg) - checks if inputString matches the argument. Returns true if match else false.
//void serialCalibrate() - prompts the user to enter ProductID, and the set of calibration values and stores them in the variables above.  

//-----------------------------------------------------//


//The following MAC addresses are derived from the BASE MAC in the EFUSE BLK0.
//#ESP_MAC_WIFI_STA
//#ESP_MAC_WIFI_SOFTAP
//#ESP_MAC_BT
//#ESP_MAC_ETH
//
//For 2 universal MAC addresses, the BT and Wifi are both enabled. Ethernet is disabled. Here:
//#ESP_MAC_WIFI_STA=ESP_MAC_WIFI_SOFTAP=BASE MAC.
//#ESP_MAC_BT=BASE MAC+1
//
//For 4 Universal MAC addresses, the BT, Wifi and ethernet are all enabled. Here:
//#ESP_MAC_WIFI_STA=BASE MAC.
//#ESP_MAC_WIFI_SOFTAP=BASE MAC+1
//#ESP_MAC_BT=BASE MAC+2
//#ESP_MAC_ETH=BASE MAC+3

//esp_efuse_mac_get_default()

/*sensor variables*/
//const int currentPin = 34;      // ACS712 pin to Arduino A4 port
//const int voltagePin = 35;
const int motionPin = 26;
const int tempPin = 35;

//old current sensor stuff
//const unsigned long sampleTime = 100000UL;                           // sample over 100ms, it is an exact number of cycles for both 50Hz and 60Hz mains
//const unsigned long numSamples = 250UL;                               // choose the number of samples to divide sampleTime exactly, but low enough for the ADC to keep up
//const unsigned long sampleInterval = sampleTime/numSamples;  // the sampling interval, must be longer than then ADC conversion time

//short adc_zero_c; 
//short adc_zero_v;

float currentRMS = 0;
float voltageRMS = 115.0;
String currentRMSString;
String voltageRMSString;

String motionString;
String tempString;
/*sensor variables*/

char ssidAP[] = "PreeHoot-";

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
const byte sLED  =  27;//pree-2: 12 ;pree-1: 13; //27  //CLED //formerly c/nLED //old ver = 13 //new version 12
const byte cButt = 26; //14 //SBUTT //formerly s/wbutton
const byte cLED   = 27;  //26 //SLED  //formerly s/wLED //old ver = 14

volatile int cnIntCnt;
volatile int swIntCnt;

int cnNumInts;
int swNumInts;

//portMUX_TYPE cnMux = portMUX_INITIALIZER_UNLOCKED;
//portMUX_TYPE swMux = portMUX_INITIALIZER_UNLOCKED;

unsigned char butTimer = 0;
unsigned long butStartTime;

unsigned long iniMillis;

int relayPin = 25;
int relayPinState = 0;
String relayPinString;
/*string stuff*/
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
  pinMode(motionPin, INPUT);
  pinMode(tempPin, INPUT);
  bool TEST = false;

  digitalWrite(cLED, HIGH);
  
  delay(3000);

  digitalWrite(cLED, LOW);

  if(Serial.available())    //kriston edit 19/2/19
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
  
  
  

  //macAddString = WiFi.macAddress();
  // macAddString = "13:80:90:56:FD:3F";
  esp_efuse_mac_get_default(efuse_mac_int);
  //char efuse_mac_char[6];
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

  ssid = "MonaConnect";
  pass = "";
  
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
//    adc_zero_c = determineVQ(currentPin);
  }

 // mode = AP;

  preferences.begin("my-app", false);    //initiialise an "App"
  productID = preferences.getString("productID");
  voltageM_string = preferences.getString("voltageM");
  voltageC_string = preferences.getString("voltageC");
  currentM_string = preferences.getString("currentM");
  currentC_string = preferences.getString("currentC");
  preferences.end();

  strcat(ssidAP, productID.c_str());  //added by kriston
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
    preeMode();
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
  getMotionData();
  getTempData();
  
  swPollCheck();
  updateRelayPostStatus();
  
  preePost();
  swPollCheck();
}






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

boolean inStringMatch(String msg)   //prepends argument string with NL and CR, and returns true if string match
{
  boolean matchBool = false;

  msg +='\r'; 
  msg +='\n';

  if(inputString.equalsIgnoreCase(msg)) //kriston edit 19/2/19
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
  //digitalWrite(cLED, HIGH);
   HTTPClient http; 
   String post = "";
   post +=  "pk=";
   post += macAddString;
   post += "&m=";         // these
   post += motionString; // comment these out to disable voltage
   post += "&t=";        //these
   //post += "&v=115.7&c="; //"&v=115.7&c=15.99&r="; 
   post += tempString;
   post += "&r=";
   post += relayPinString;
   Serial.println(post);
 
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


void getMotionData()
{
  if(digitalRead(motionPin) == 1)
  {
    motionString = "1";
  }
  else
  {
    motionString = "0";
  }
}
void getTempData()
{
  tempString = "27";
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
