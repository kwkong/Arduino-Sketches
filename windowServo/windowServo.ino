/* 

Room Alert Extension

Author: Kriston Kong

*/

#include <Servo.h>

Servo servo;  

const int alertPin = 12;
const int servoPin = 5;
const int extractorPin = 4;
const int servoPowerPin = 3;

const int openPos = 70;    
const int closePos = 20;

int alertStatus = 0;
int servoState = 0;
int servoStateOld = 0;

void setup() 
{
	delay(2000);

	servo.attach(servoPin);  

	pinMode(13, OUTPUT);
	pinMode(extractorPin, OUTPUT);
	pinMode(servoPowerPin, OUTPUT);

	digitalWrite(servoPowerPin, HIGH);
	digitalWrite(extractorPin, HIGH);
}

void loop() 
{	
	alertStatus = !(digitalRead(alertPin));

	if(alertStatus == 1)
	{
		//Open window, turn on extractor

		digitalWrite(extractorPin, LOW);
		openWindow();

		digitalWrite(13, HIGH);
	}

	else 
	{
		//Close window, turn off extractor

		digitalWrite(extractorPin, HIGH);
		closeWindow();

		digitalWrite(13, LOW);
	}

		
}                    
 

void openWindow()
{
	servoState = 1;

	if (servoStateOld != servoState)
	{
		digitalWrite(servoPowerPin, LOW);
		delay(1000);
		servo.write(openPos);
		delay(1000);
		digitalWrite(servoPowerPin, HIGH);
		servoStateOld = servoState;
	}
}

void closeWindow()
{
	servoState = 0;

	if (servoStateOld != servoState)
	{
		digitalWrite(servoPowerPin, LOW);
		delay(1000);
		servo.write(closePos);
		delay(1000);
		digitalWrite(servoPowerPin, HIGH);
		servoStateOld = servoState;
	}
}
