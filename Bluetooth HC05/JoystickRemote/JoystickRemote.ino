#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX

int x = 0;
int y = 0;

boolean state = false;

void setup() {
  
  Serial.begin(9600);
  mySerial.begin(38400);

  pinMode(4, INPUT_PULLUP);

}

void loop() 
{

	// while (1)
	// {
	// 	x = analogRead(A0);
	// 	y = analogRead(A1);
	// 	Serial.print(x);
	// 	Serial.print(" , ");
	// 	Serial.println(y);
	// }

	x = analogRead(A0);
	y = analogRead(A1);

	if (!digitalRead(4))
	{
		while(!digitalRead(4));

		state = !state;

		if (state)
		{
			mySerial.write('O');
			Serial.println("open");
			delay(1000);
		}

		else 
		{
			mySerial.write('C');
			Serial.println("closed");
			delay(1000);
		}		
	}

	if (y > 600)
	{
		mySerial.write('F');
		Serial.println("forward");
		delay(30);		
	}

	if (y < 300)
	{
		mySerial.write('B');
		Serial.println("back");
		delay(30);		
	}		

	if (x > 600)
	{
		mySerial.write('R');
		Serial.println("right");
		delay(30);		
	}

	if (x < 300)
	{
		mySerial.write('L');
		Serial.println("left");
		delay(30);		
	}

	if((x < 600)&&(x > 300) && (y < 600)&&(y > 300))
	{
		mySerial.write('S');
		Serial.println("stop");
		delay(30);
	}

}
