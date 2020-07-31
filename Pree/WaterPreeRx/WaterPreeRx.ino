#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

#define timerout 10000

LiquidCrystal_I2C lcd(0x3F, 16, 2);
SoftwareSerial mySerial(3, 2);

int maxDepth = 30;
int oldBarNum = 0;
int newBarNum = 0;

int num = 0;

String inputString= "";
boolean stringComplete = false; 

void setup()
{

	Serial.begin(9600);
	mySerial.begin(38400);
	
	// initialize the LCD
	lcd.begin();
	lcd.backlight();

	lcd.print("PreeLabs");
	lcd.setCursor(0,1);
	lcd.print("Water Monitor");
	lcd.clear();
	delay(2000);
}

void loop()
{
	printDepth();
	mySerialEvent();
}

int getDepth()
{
	int depth;
	char val;

	if(mySerial.available())
	{ 
		val = mySerial.read();
		if(val == 's')
		{
			Serial.print("got s");
			depth = mySerial.read();
		//}
		//depth = mySerial.read();
		Serial.print(depth);
		mySerial.write('k');
		}
	

	depth = map(depth, 0, 30, 0, 12);

	Serial.print("         calcAfter: ");
	Serial.println(depth);

	return depth;
	}
	else
	{
		return 1000;
	}

}

void printDepth()
{
	lcd.setCursor(0,0);
	lcd.print("Capacity:");

	lcd.setCursor(0,1);
	lcd.print('E');

	lcd.setCursor(15,1);
	lcd.print('F');

	if (stringComplete == true)
	{
		stringComplete = false;

		if ((num < 30) && (num > 0))
		{
			num = map(num, 0, 30, 12, 0);
			printBar(num);
			//Serial.println(num);
			oldBarNum = num;
		}


	}


	//Serial.println(getDepth());

	//oldBarNum = getDepth();
	
	//delay(500);
	//lcd.clear();
}

void printBar(int barNum)
{
	for(int i=0;i<barNum;i++)
	{
		lcd.setCursor((i+2),1);
		lcd.write(255);
	}

	delay(100);

	 if (num != oldBarNum)
	 {
		for(int i=0;i<13;i++)
		{
			lcd.setCursor((i+2),1);
			lcd.print(" ");
		}
	}
}

void mySerialEvent() 
{
	if (mySerial.available()) 
	{
		//Serial.print(mySerial.read());

		char inChar = mySerial.read();

		//Serial.print(inChar);

		inputString += (char)inChar;

		if (inChar == '\n') 
		{
			stringComplete = true;
			num = inputString.toInt();
			inputString = "";
			// Serial.print("Rx: ");
			// Serial.println(num);
		}

		//Serial.println("Serial available");
	}
}