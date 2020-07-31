#include <SPI.h>
#include <nRF24l01.h>
#include <RF24.h>

RF24 radio(7,8);
const byte address[6] = "00006";

int analogPin = A0;
int sensorVal = 0;
float voltage, temp;
char Data[32]="";
char TempStr[4];

void setup()
{
	//pinMode(LED_BUILTIN, OUTPUT);
	radio.begin();
	radio.openWritingPipe(address);
	radio.setPALevel(RF24_PA_HIGH);
	radio.stopListening();
	analogReference(DEFAULT);
}    

void loop()
{
	sensorVal = analogRead(analogPin);
	voltage = sensorVal*(5.0/1023.0);
	temp = voltage*100;

	dtostrf(temp,3  ,1,TempStr);
	strcpy(Data, "My Temperature: ");
	strcat(Data, TempStr);

	radio.write(&Data, sizeof(Data));

	//digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
	delay(1000);
}

