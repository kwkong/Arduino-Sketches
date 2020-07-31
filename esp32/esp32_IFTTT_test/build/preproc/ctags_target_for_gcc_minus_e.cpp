# 1 "c:\\Arduino\\My Sketches\\esp32_IFTTT_test\\esp32_IFTTT_test.ino"
# 1 "c:\\Arduino\\My Sketches\\esp32_IFTTT_test\\esp32_IFTTT_test.ino"
# 2 "c:\\Arduino\\My Sketches\\esp32_IFTTT_test\\esp32_IFTTT_test.ino" 2
# 3 "c:\\Arduino\\My Sketches\\esp32_IFTTT_test\\esp32_IFTTT_test.ino" 2

String ssid = "MonaConnect";
String password = "";
char stateChar = '0';


void setup()
{
    pinMode(25, 0x02);
    Serial.begin(115200);
    delay(4000); //Delay needed before calling the WiFi.begin

    WiFi.begin("PowerPreeHS", "12345679");

    while (WiFi.status() != WL_CONNECTED)
    { //Check for the connection
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }

    Serial.println("Connected to the WiFi network");

}

void loop()
{

    if(WiFi.status()== WL_CONNECTED)
    { //Check WiFi connection status
        HTTPClient http;

        http.begin("http://198.199.80.189:1025/espPost"); //Specify destination for HTTP request
        http.addHeader("Content-Type", "application/x-www-form-urlencoded"); //Specify content-type header

        int httpResponseCode = http.POST("POSTING from ESP32"); //Send the actual POST request

        if(httpResponseCode>0)
        {
            String response = http.getString(); //Get the response to the request
            Serial.println(httpResponseCode); //Print return code
            Serial.println(response); //Print request answer
            stateChar = response.charAt(response.indexOf('=')+1);
        }

        else
        {
            Serial.print("Error on sending POST: ");
            Serial.println(httpResponseCode);
        }

        http.end(); //Free resources
    }

    else
    {
        Serial.println("Error in WiFi connection");
    }

    if(stateChar == '1')
    {
        digitalWrite(25, 0x1);
    }

    else
    {
        digitalWrite(25, 0x0);
    }

    //delay(500);
}
