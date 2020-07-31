# 1 "c:\\Arduino\\My Sketches\\esp32clientTest\\esp32clientTest.ino"
# 1 "c:\\Arduino\\My Sketches\\esp32clientTest\\esp32clientTest.ino"

# 3 "c:\\Arduino\\My Sketches\\esp32clientTest\\esp32clientTest.ino" 2
# 4 "c:\\Arduino\\My Sketches\\esp32clientTest\\esp32clientTest.ino" 2
# 5 "c:\\Arduino\\My Sketches\\esp32clientTest\\esp32clientTest.ino" 2


WiFiServer server(80);


void setup()
{
    Serial.begin(115200);
    Serial.println();
    Serial.println("Configuring access point...");
    WiFi.softAP("ESP32", "");
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    server.begin();

    Serial.println("Server started");
}

void loop()
{
    WiFiClient client = server.available(); // listen for incoming clients

    if (client)
    {
        Serial.println("New Client.");
        String currentLine = ""; // make a String to hold incoming data from the client

        while (client.connected())
        {
            if (client.available())
            {
                char c = client.read();
                Serial.write(c);

                if (c == '\r')
                {
                    if (currentLine.endsWith("GET /H HTTP/1.1"))
                    {
                        Serial.println("*********HIGH***********");
                        while(1);
                    }

                    currentLine = "";
                    break;
                }

                else
                {
                    currentLine += c;
                }
            }
        }
    }

    client.stop();
    Serial.println("Client Disconnected.");
}
