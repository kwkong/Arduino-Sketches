# 1 "c:\\Arduino\\My Sketches\\pree32CT-2019-8-8\\pree32CT-2019-8-8.ino"
# 1 "c:\\Arduino\\My Sketches\\pree32CT-2019-8-8\\pree32CT-2019-8-8.ino"
# 2 "c:\\Arduino\\My Sketches\\pree32CT-2019-8-8\\pree32CT-2019-8-8.ino" 2

# 4 "c:\\Arduino\\My Sketches\\pree32CT-2019-8-8\\pree32CT-2019-8-8.ino" 2

# 6 "c:\\Arduino\\My Sketches\\pree32CT-2019-8-8\\pree32CT-2019-8-8.ino" 2

# 8 "c:\\Arduino\\My Sketches\\pree32CT-2019-8-8\\pree32CT-2019-8-8.ino" 2



#define DEBUG true
#define sampleRate 60
#define currentZero 0.2
#define ippsSize 5


//-----------------KRISTON ADDED-----------------------//

float voltageM_float, currentM_float, voltageC_float, currentC_float; //variables to store calibration data
String productID, voltageM_string, currentM_string, voltageC_string, currentC_string; //String to hold product ID
String inputString; //String necessary for functions: inStringMatch, inString.

//Added three new functions
//String inString() - waits for user to enter a string via serial (after hitting enter), then returns that string.
//boolean inStringMatch(String msg) - checks if inputString matches the argument. Returns true if match else false.
//void serialCalibrate() - prompts the user to enter ProductID, and the set of calibration values and stores them in the variables above.  

//-----------------------------------------------------//


#define sButt 27
#define sLED 14
#define cButt 26
#define cLED 12
#define relayPin 25
#define currentPin 34
#define voltagePin 35

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
unsigned long iniMillis;

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

int counter[60]; //prev 400
int currents[60]; //prev 200
int voltages[60]; //prev 200

int vpp;
int ipp;

unsigned long vTime;
unsigned long iTime;
unsigned long pTime1;
unsigned long pTime2;

float currentBuffer;
float voltageBuffer;

String currentRMSString;
String voltageRMSString;

float currentRMS = 0;
float voltageRMS = 115.0;

            int ipps[5]; //new variables 2019/8/2
            int ippCount = 0;
            int ippAvg = 0;
            int ippAvgOld = 0;

void setup()
{
    Serial.begin(115200);
    Serial.println("yo");

    pinMode(25, 0x02);
    pinMode(14, 0x02);
    pinMode(12, 0x02);
    pinMode(26, 0x05);
    pinMode(27, 0x05);

    bool TEST = false;

    digitalWrite(12, 0x1);
    delay(3000);
    digitalWrite(12, 0x0);

    if (Serial.available()) //kriston edit 19/2/19
    {
        ssid = Serial.readString();

        if (ssid.equalsIgnoreCase("test\r\n"))
        {
            TEST = true;
        }

        ssid = "";
    }

    while ((digitalRead(26) == 0) && (digitalRead(27) == 0))
    {
        TEST = true;
        digitalWrite(14, 0x1);
        digitalWrite(12, 0x1);
    }

    digitalWrite(14, 0x0);
    digitalWrite(12, 0x0);

    esp_efuse_mac_get_default(efuse_mac_int);

    String str0;
    String str1;
    String str2;
    String str3;
    String str4;
    String str5;
    String str6;

    str0 = String(efuse_mac_int[0], 16);
    str1 = String(efuse_mac_int[1], 16);
    str2 = String(efuse_mac_int[2], 16);
    str3 = String(efuse_mac_int[3], 16);
    str4 = String(efuse_mac_int[4], 16);
    str5 = String(efuse_mac_int[5], 16);
    str6 = String(efuse_mac_int[6], 16);

    macAddString += str0;
    macAddString += ":";
    macAddString += str1;
    macAddString += ":";
    macAddString += str2;
    macAddString += ":";
    macAddString += str3;
    macAddString += ":";
    macAddString += str4;
    macAddString += ":";
    macAddString += str5;

    Serial.println(macAddString);

    Serial.println("getting ssid from eeprom"); //getting ssid from eeprom
    preferences.begin("my-app", false); //initiialise an "App"
    ssid = preferences.getString("ssid"); // get whatever is stored in that location
    pass = preferences.getString("pass");
    preferences.end();

    if (TEST == false)
    {
        Serial.println(ssid);
        if (ssid == 0) //checking to see if an ssid was stored previously
        {
            Serial.println("no ssid found"); // no ssid found
            mode = AP;
        }

        else //if ssid was set previously
        {
            mode = pree;
        }
    }

    if (TEST == true)
    {
        mode = preeTest;
    }

    preferences.begin("my-app", false); //initiialise an "App"
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

    strcat(ssidAP, productID.c_str()); //added by kriston
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

    ippAvgOld = getIpp(60); //sets initial ipp 
}


void cnPollCheck()
{
    if (digitalRead(26) == 0)
    {
        unsigned long buttStartTime = millis();

        while (digitalRead(26) == 0)
        {
            if (millis() > buttStartTime + 3000)
            {
                if (mode != AP)
                {
                    mode = AP;
                }

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
    if (digitalRead(27) == 0)
    {
        unsigned long buttStartTime = millis();

        while (digitalRead(27) == 0)
        {
            if (millis() > buttStartTime + 1500)
            {
                break;
            }
        }

        digitalWrite(25, !digitalRead(25));
        digitalWrite(14, digitalRead(25));
        updateRelayPostStatus();

        while (digitalRead(27) == 0);
    }

}

void loop()
{
    cnPollCheck();
    swPollCheck();

    if (mode == AP)
    {
        APMode();
    }

    if (mode == pree)
    {
        preeMode();
    }

    if (mode == preeTest)
    {
        preeTestMode();
    }
}

void APMode() {

    digitalWrite(12, 0x1);
    delay(500);
    digitalWrite(12, 0x0);
    delay(500);
    digitalWrite(12, 0x1);
    delay(500);
    digitalWrite(12, 0x0);

    WiFi.mode(WIFI_MODE_STA);
    WiFi.softAP(ssidAP, passAP);

    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);

    server.begin();

    int value = 0;
    int check = 0;
    int stater = 0;
    String responseString = "";
    while (check == 0) //set condition for this
    {
        WiFiClient client = server.available(); // listen for incoming clients

        int s = 0;
        while (s < 2)
        {
            digitalWrite(12, 0x1);
            delay(300);
            digitalWrite(12, 0x0);
            delay(300);
            s++;
        }

        if (client) // if you get a client,
        {
            Serial.println("New Client."); // print a message out the serial port
            client.println("Status: 200"); //EDing this now
            client.println("Access-Control-Allow-Origin: *");
            client.println("Access-Control-Allow-Methods: GET,POST,DELETE");
            //   client.println("Access-Control-Allow-Headers: X-Requested-With, Phant-Private-Key");
            client.println("Content-Type: text/html");
            client.println("Connection: close");

            String currentLine = ""; // make a String to hold incoming data from the client

            while (client.connected()) // loop while the client's connected
            {
                int c = 0;

                if (stater == 0)
                {
                    while ((c < 2))
                    {
                        digitalWrite(12, 0x1);
                        delay(300);
                        digitalWrite(12, 0x0);
                        delay(300);
                        c++;
                    }

                    digitalWrite(12, 0x1);
                    delay(500);
                    digitalWrite(12, 0x0);
                }

                if (digitalRead(26) == 0)
                {
                    while (digitalRead(26) == 0);
                    check = 2;
                    break;
                }

                if (client.available()) // if there's bytes to read from the client,
                {
                    stater = 1;
                    char c = client.read(); // read a byte, then
                    responseString += c;

                    Serial.write(c); // print it out the serial monitor

                    if (c == '~')
                    {
                        stater = 0;
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println();
                        client.println("got message from app");
                        int i = 0;

                        while (i < 3)
                        {
                            digitalWrite(12, 0x1);
                            delay(500);
                            digitalWrite(12, 0x0);
                            delay(500);
                            i++;
                        }

                        check = 1;
                        break;
                    }
                }

                if (!(client.connected()))
                {
                    break;
                }

            }

            // close the connection:
            client.stop();
            Serial.println("Client Disconnected.");
            digitalWrite(12, 0x1);
            delay(500);
            digitalWrite(12, 0x0);
            delay(500);
            digitalWrite(12, 0x1);
            delay(500);
            digitalWrite(12, 0x0);
        }
    }

    if (check == 1)
    {
        //parse for ssid and pass
        int tokStart = responseString.indexOf("SSID");
        int tokEnd = responseString.indexOf("&");
        //String messageString = responseString.substring(tokStart+5, tokEnd);
        ssid = responseString.substring(tokStart + 5, tokEnd);

        tokStart = responseString.indexOf("pass");
        //tokEnd = responseString.indexOf("&");
        tokEnd = responseString.indexOf("&u");
        pass = responseString.substring(tokStart + 5, tokEnd);
        //pass = "";
        tokStart = responseString.indexOf("uk");
        tokEnd = responseString.indexOf("~");
        uk = responseString.substring(tokStart + 3, tokEnd);

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

        //FILL IN HERE TO HARDCODE SSID AND PASS
        // ssid = "PowerPreeHS";  //get from parsing
        // pass = "12345679";             //get from parsing

        WiFi.mode(WIFI_MODE_STA);

        Serial.println("converting String to CharArray for WIFI.begin");
        char ssidChar[20];
        char passChar[20];
        ssid.toCharArray(ssidChar, 20);
        pass.toCharArray(passChar, 20);

        //attempt to connect
        Serial.println("Attempting to connect now");
        WiFi.begin(ssidChar, passChar);

        unsigned long timeout = 5000;
        unsigned long inimillis = millis();
        while ((WiFi.status() != WL_CONNECTED) && millis() < inimillis + timeout)
        {
            int w = 0;
            while (w < 4)
            {
                digitalWrite(12, 0x1);
                delay(100);
                digitalWrite(12, 0x0);
                delay(100);
                w++;
            }

            Serial.println("trying...");
            delay(500);
        }


        if (WiFi.status() == WL_CONNECTED) //if connection succesful
        {
            digitalWrite(12, 0x1);
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
            int httpResponseCode = http.POST(inipost); //Send the actual POST request

            if (httpResponseCode > 0)
            {
                String response = http.getString(); //Get the response to the request

                Serial.println(httpResponseCode); //Print return code
                Serial.println(response); //Print request answer
            }

            else
            {
                Serial.print("Error on sending POST: ");
                Serial.println(httpResponseCode);
            }

            http.end(); //Free resources

            digitalWrite(12, 0x1);
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
            int u = 0;
            while (u < 5)
            {
                digitalWrite(12, 0x1);
                delay(300);
                digitalWrite(12, 0x0);
                delay(300);
                u++;
            }

            mode = AP;
        }

        digitalWrite(12, 0x1);
        //Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        Serial.println("MAC address: ");
        Serial.println(WiFi.macAddress());
    }

    if (check == 2)
    {

    }

}

void preeMode()
{
    getRawData();

    swPollCheck();
    updateRelayPostStatus();

    preePost();
    swPollCheck();
}

void preeTestMode()
{
    Serial.println("in test mode");
    int c = 0;
    int s = 0;
    int avgSize = 20;
    int avgRaw = 0;
    float avgCal = 0;
    int i = 0;
    int exitl = 0;

    digitalWrite(14, 0x1);
    digitalWrite(12, 0x1);
    digitalWrite(25, 0x1);
    delay(2000);

    digitalWrite(14, 0x0);
    digitalWrite(12, 0x0);
    digitalWrite(25, 0x0);
    delay(2000);

    Serial.println("press cButt"); //kriston edit 19/2/19
    digitalWrite(12, 0x1);
    unsigned long startTim = millis();
    while (digitalRead(26) == 0x1) // && (millis() < startTim + 5000));
    {
        if (Serial.available())
        {
            while (Serial.available())
            {
                Serial.read();
            }

            break;
        }
    }

    Serial.println("pressed");
    digitalWrite(14, 0x1);
    digitalWrite(12, 0x0);
    Serial.println("press sButt");
    startTim = millis();

    while (digitalRead(27) == 0x1) // && (millis() < startTim + 5000));
    {
        if (Serial.available())
        {
            while (Serial.available())
            {
                Serial.read();
            }

            break;
        }
    }

    digitalWrite(25, 0x1);
    int testModeMode = 0;

    while (1)
    {
        digitalWrite(14, !digitalRead(14)); //toggle sLED for feedback on loop

        if (Serial.available())
        {
            inputString = inString();

            if (inStringMatch(""))
                testModeMode = 0;

            if (inStringMatch("vpp"))
                testModeMode = 1;

            if (inStringMatch("ipp"))
                testModeMode = 2;

            if (inStringMatch("ippraw"))
                testModeMode = 3;

            if (inStringMatch("vppraw"))
                testModeMode = 4;

            if (inStringMatch("bothraw"))
                testModeMode = 5;

            if (inStringMatch("calibrate"))
                testModeMode = 6;

            if (inStringMatch("clear"))
                testModeMode = 7;

            inputString = "";
        }

        if (testModeMode == 0) // regular data posts
        {
            getVals();
            calcVals();
            printVals();
        }

        if (testModeMode == 1) //shows avg vpp and voltage reading
        {
            int avgRaw = 0;
            float avgCal = 0;
            int i = 0;

            while (i < avgSize)
            {
                getVals();
                calcVals();
                avgRaw += vpp;
                avgCal += voltageBuffer;
                i++;
            }

            avgRaw = avgRaw / avgSize; //average vpp readings for size
            avgCal = avgCal / avgSize; //average 
            Serial.print("avg vpp= ");
            Serial.print(avgRaw);
            Serial.print(", avg v= ");
            Serial.println(avgCal);
        }

        if (testModeMode == 2) //shows avg vpp and voltage reading
        {
            int avgRaw = 0;
            float avgCal = 0;
            int i = 0;

            while (i < avgSize)
            {
                getVals();
                calcVals();
                avgRaw += ipp;
                avgCal += currentBuffer;
                i++;
            }

            avgRaw = avgRaw / avgSize;
            avgCal = avgCal / avgSize;
            Serial.print("avg IPP= ");
            Serial.print(avgRaw);
            Serial.print(", avg C= ");
            Serial.println(avgCal);
        }

        if (testModeMode == 3) //current graph
        {
            digitalWrite(14, 0x0);
            digitalWrite(12, 0x1);
            printCurrents();
        }

        if (testModeMode == 4) //voltage graph
        {
            digitalWrite(14, 0x1);
            digitalWrite(12, 0x0);
            printVoltages();
        }

        if (testModeMode == 5) //both graph
        {
            digitalWrite(14, 0x1);
            digitalWrite(12, 0x1);
            getVals();
            printBoth();
        }

        if (testModeMode == 6) //enter calibrated values
        {
            serialCalibrate();
            delay(5000);
            testModeMode = 0;
        }

        if (testModeMode == 7) //enter calibrated values
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

//----------------KRISTON CALIBRATION CODE---------------------------//

String inString() //Waits for the user to send command via Serial
{
    boolean serialComplete = false;
    String inString = "";

    while (serialComplete == false)
    {
        while (Serial.available())
        {
            char inChar = (char) Serial.read();
            inString += inChar;

            if (inChar == '\n')
            {
                serialComplete = true;
            }
        }
    }

    return inString;
}

boolean inStringMatch(String msg) //prepends argument string with NL and CR, and returns true if string match
{
    boolean matchBool = false;

    msg += '\r';
    msg += '\n';

    if (inputString.equalsIgnoreCase(msg)) //kriston edit 19/2/19
    {
        matchBool = true;
    }

    else
    {
        matchBool = false;
    }

    return matchBool;
}

void serialCalibrate() //Prompts user to enter values then saves em'
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

    Serial.println("Enter Current intercept(C)");
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
    int ippMax = 0;
    int ippMin = 4000;
    int replaceCount = 2; //amount of max and min values that get replaced 

    int ippSpike[3];
    int ippCalc = 0;

    for (int i = 0; i < 3; i++)
    {
        ippSpike[i] = getIpp(60);
    }

    for(int i=0; i<3; i++) //finds the biggest ipp in the array
    {
        if(ippSpike[i] > ippMax)
        {
            ippMax = ippSpike[i];
        }
    }

    for(int i=0; i<3; i++) //finds the biggest ipp in the array
    {
        if(ippSpike[i] < ippMax)
        {
            ippCalc = ippSpike[i];
            break;
        }
    }

    ipps[ippCount] = ippCalc;
    ippCount++;

    if(ippCount > (5 -1)) //averages every 10 loops
    {
        for(int i=0; i<5; i++) //calculates the average
        {
            ippAvg += ipps[i];
        }

        ippAvg /= 5;

        if ((ippAvg > (ippAvgOld +3)) || (ippAvg < (ippAvgOld -3)))
        {
            ippAvgOld = ippAvg;
        }

        ippAvg = 0;
        ippCount =0;
    }

    ipp = ippAvgOld;
}


int getIpp(int sampleNumber)
{
    int vMax = 0;
    int vMin = 3000;
    int iMax = 0;
    int iMin = 3000;
    int _ipp = 0;

   // unsigned long exTime = millis();

    for(int i=0; i<sampleNumber; i++)
    {
        voltages[i] = analogRead(35);
        currents[i] = analogRead(34);

        if (vMax < voltages[i])
        {
            vMax = voltages[i];
            vTime = micros();
        }

        if (iMax < currents[i])
        {
            iMax = currents[i];
            iTime = micros();
        }

        if (vMin > voltages[i])
        {
            vMin = voltages[i];
        }

        if (iMin > currents[i])
        {
            iMin = currents[i];
        }

        delayMicroseconds(980);
    }

    _ipp = iMax - iMin;

  //  exTime = millis() - exTime;

    // Serial.println("");
    // Serial.print("EXECUTION TIME:");
    // Serial.print(exTime);
    // Serial.println("");

    return _ipp;
}

void printVals()
{
    Serial.print("ipp:");
    Serial.print(ipp);
    Serial.print(",current:");
    Serial.print(currentBuffer);
    Serial.print(",vpp:");
    Serial.print(vpp);
    Serial.print(",voltage:");
    Serial.print(voltageBuffer);
    Serial.print(",pTime1:");
    Serial.print(pTime1);
    Serial.print(",pTime2:");
    Serial.println(pTime2);
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

    WiFi.mode(WIFI_MODE_STA);

    Serial.print("converting String to CharArray for WIFI.begin");
    char ssidChar[20];
    char passChar[20];
    ssid.toCharArray(ssidChar, 20);
    pass.toCharArray(passChar, 20);

    Serial.println("Attempting to connect now");
    WiFi.begin(ssidChar, passChar);
}

void preePost()
{
    if (WiFi.status() == WL_CONNECTED) //Check WiFi connection status
    {
        digitalWrite(12, 0x1);
        HTTPClient http;
        String post = "";
        post += "pk=";
        post += macAddString;
        post += "&v="; // these
        post += voltageRMSString; // comment these out to disable voltage
        post += "&c="; //these
        post += currentRMSString;
        post += "&r=";
        post += relayPinString;

        // http.begin("http://posttestserver.com/post.php");  //Specify destination for HTTP request
        http.begin("http://www.powerpree.com/methods/postpree");
        //http.addHeader("Content-Type", "text/plain");             //Specify content-type header
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        int httpResponseCode = http.POST(post); //Send the actual POST request

        if (httpResponseCode > 0)
        {
            String response = http.getString(); //Get the response to the request

            Serial.println(httpResponseCode); //Print return code
            Serial.println(response); //Print request answer
            responseStringPree = response;
            int tokStart = responseStringPree.indexOf("toggle");

            String messageString = responseStringPree.substring(tokStart + 7, tokStart + 8);
            params1 = messageString;
            Serial.println(params1);
            switchRelay();
        }

        else
        {
            digitalWrite(12, 0x1);
            delay(100);
            digitalWrite(12, 0x0);
            delay(100);
            digitalWrite(12, 0x1);
            delay(100);
            digitalWrite(12, 0x0);

            Serial.print("Error on sending POST: ");
            Serial.println(httpResponseCode);
        }

        http.end(); //Free resources
    }

    else
    {
        digitalWrite(12, 0x0);
        Serial.println("Error in WiFi connection");
        connectAttempt();
    }
}

void connectAttempt()
{
    WiFi.mode(WIFI_MODE_STA);

    Serial.println("converting String to CharArray for WIFI.begin");
    char ssidChar[20];
    char passChar[20];
    ssid.toCharArray(ssidChar, 20);
    pass.toCharArray(passChar, 20);
    //attempt to connect
    Serial.println("Attempting to connect now");
    WiFi.begin(ssidChar, passChar);

    unsigned long timeout = 5000;
    unsigned long inimillis = millis();
    while ((WiFi.status() != WL_CONNECTED) && millis() < inimillis + timeout)
    {
        int w = 0;
        while (w < 4)
        {
            digitalWrite(12, 0x1);
            delay(100);
            digitalWrite(12, 0x0);
            delay(100);
            w++;
        }

        Serial.println("trying...");
        swPollCheck();
        delay(500);
        swPollCheck();
    }


    if (WiFi.status() == WL_CONNECTED) //if connection succesful
    {
        Serial.println("Connection succesful");
        Serial.println(macAddString);
        digitalWrite(12, 0x1);
    }

    else
    {
        Serial.println("Connection unsuccesful");
        digitalWrite(12, 0x0);
    }
}

void getRawData() //TO DO
{
    getVals();

    if (true == true)
    {
        printVals();
    }

    calcVals();

    if (true)
    {
        Serial.print(currentBuffer);
        Serial.println("A,");
        Serial.print(voltageBuffer);
        Serial.println("V");
    }

    if (digitalRead(25) == 1)
    {
        currentRMSString = String(currentBuffer, 2);
        voltageRMSString = String(voltageBuffer, 2);
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
    //voltageBuffer = (vpp * voltageM_float) + voltageC_float;
    voltageBuffer = 110;

    if (currentBuffer < 0.2)
    {
        currentBuffer = 0;
    }
}

void switchRelay() //TO DO
{
    if (params1 == "1") //??? //suh fool dawg?
    {
        if (digitalRead(25) == 0)
        {
            digitalWrite(25, 0x1);
            digitalWrite(14, digitalRead(25));
            relayPinState = 1;
            Serial.println("switch on");
        }
        else
        {
            digitalWrite(25, 0x0);
            digitalWrite(14, digitalRead(25));
            relayPinState = 0;
            Serial.println("switch off");
        }
    }

    else
    {
        Serial.println("no switch");
    }
}
void updateRelayPostStatus()
{
    if (digitalRead(25) == 0)
    {
        relayPinString = "-1";
    }

    else
    {
        relayPinString = "1";
    }
}

void printCurrents()
{
    int count1 = 0;
    int buff;
    int state = 0;

    while (count1 <= 60) //80 before
    {
        if (state == 0)
        {
            state = 1;
        }

        else
        {
            buff = currents[count1];
        }

        Serial.println(buff);
        count1++;
    }
}
void printVoltages()
{
    int count1 = 0;
    int buff;
    int state = 0;

    while (count1 <= 60) //80 before
    {
        if (state == 0)
        {
            state = 1;
        }

        else
        {
            buff = voltages[count1]; ///recomment for voltage
            Serial.println(buff); ///recomment for voltage
            count1++;
        }
    }
}
void printBoth()
{
    int count1 = 0;
    int buff;
    int state = 0;

    while (count1 <= 60) //80 before
    {
        if (state == 0)
        {
            state = 1;
        }

        else
        {
            buff = currents[count1];
        }

        Serial.print(buff);
        Serial.print(","); ///recomment for voltage
        buff = voltages[count1]; ///recomment for voltage
        Serial.println(buff); ///recomment for voltage

        count1++;
    }
}

void getFreq() //uses 
{
    int state = 0;
    unsigned long endTime;
    unsigned long startTime;

    while (state != 10)
    {
        int rVoltage = analogRead(35);
        if (rVoltage == 0) //looks for the first 0 to cancel first incomplete pulse
        {
            //startTime = micros();
            state = 1;
        }

        while (state == 1) //looks for rising edge of first pulse
        {
            rVoltage = analogRead(35);

            if (rVoltage > 5)
            {
                state = 2;
            }
        }

        while (state == 2) //looks for first 0 after pulse to start clock
        {
            rVoltage = analogRead(35);

            if (rVoltage == 0)
            {
                startTime = micros();
                state = 3;
            }
        }

        while (state == 3) //looks for first rising edge after pulse to be measured
        {
            rVoltage = analogRead(35);

            if (rVoltage > 5)
            {
                state = 4;
            }
        }

        while (state == 4) //looks for second 0 to take period
        {
            rVoltage = analogRead(35);

            if (rVoltage == 0)
            {
                endTime = micros();
                state = 5;
            }
        }

        while (state == 5)
        {
            float period = endTime - startTime;
            float freq = 1 / period;
            freq = freq * 1000000;

            Serial.print("period: ");
            Serial.println(period);
            Serial.print("freq: ");
            Serial.println(freq);
            state = 10;
        }
    }
}
