#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

WebServer WebServer;
DNSServer DNSServer;
WiFiServer server(80);

bool DNSActive = false;

void setup() {

  Serial.begin(115200);

  WiFi.begin();


  WebServer.begin();

      WiFi.softAP("esp32ap", "12345678", 1, 0);
    while (WiFi.softAPIP() == IPAddress(0, 0, 0, 0))
      delay(100);
    if (WiFi.softAPConfig(IPAddress(192, 168, 244, 1), IPAddress(192, 168, 244, 1), IPAddress(255, 255, 255, 0))) {
      Serial.printf("SoftAP %s\n", WiFi.softAPIP().toString().c_str());
      DNSServer.setErrorReplyCode(DNSReplyCode::NoError);
      DNSServer.start(53, "*", WiFi.softAPIP());
      Serial.println("DNS server started");
      DNSActive = true;
    }
    else
      Serial.println("WiFi.softAPConfig failed");
      
}




void loop() {

WiFiClient client = server.available();

  if (DNSActive) {
    DNSServer.processNextRequest();
  }
  WebServer.handleClient();

  if (client)
  	Serial.println("New client");
}

