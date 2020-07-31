/*Program to control LED (ON/OFF) from ESP32 using Serial Bluetooth
 * Thanks to Neil Kolbans for his efoorts in adding the support to Arduino IDE
 * Turotial on: www.circuitdigest.com 
 */

#include "BluetoothSerial.h" //Header File for Serial Bluetooth, will be added by default into Arduino

BluetoothSerial ESP_BT; //Object for Bluetooth

char incoming;
String msg = "";
boolean stringComplete = false;


void setup() {
  Serial.begin(9600); //Start Serial monitor in 9600
  ESP_BT.begin("ESP32_Bluetooth"); //Name of your Bluetooth Signal
  Serial.println("Bluetooth Device is Ready to Pair");

}

void loop() 
{
	if (ESP_BT.available()) //Check if we receive anything from Bluetooth
	{
		msg = "";

		while (!stringComplete)
		{
			incoming = ESP_BT.read(); //Read what we recevive 
			msg += incoming;
			delay(5);

			if (incoming  == '\n')
			{
				stringComplete = true;
			}
		}   
	}

	if (stringComplete)
	{
		Serial.println(msg);
		stringComplete = false;
	}
}