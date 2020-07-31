#include <PinChangeInt.h>


#define pcIntPin1 4
#define pcIntPin2 5
#define pcIntPin3 6
#define pcIntPin4 7

volatile int val = 0;


void pcIntVect1() 
{
  val = 1;
}

void pcIntVect2() 
{
  val = 2;
}

void pcIntVect3() 
{
  val = 3;
}

void pcIntVect4() 
{
  val = 4;
}

void setup() 
{
	pinMode(pcIntPin1, INPUT_PULLUP);
	PCintPort::attachInterrupt(pcIntPin1, pcIntVect1, FALLING);
	pinMode(pcIntPin2, INPUT_PULLUP);
	PCintPort::attachInterrupt(pcIntPin2, pcIntVect2, FALLING);
	pinMode(pcIntPin3, INPUT_PULLUP);
	PCintPort::attachInterrupt(pcIntPin3, pcIntVect3, FALLING);
	pinMode(pcIntPin4, INPUT_PULLUP);
	PCintPort::attachInterrupt(pcIntPin4, pcIntVect4, FALLING);
  
	Serial.begin(9600);

	Serial.println("Ready");
}

void loop() 
{
	switch (val) 
	{
		case 1: 
			Serial.println("INT 1 Triggered");
			val = 0;
			break;

		case 2: 
			Serial.println("INT 2 Triggered");
			val = 0;
			break;

		case 3: 
			Serial.println("INT 3 Triggered");
			val = 0;
			break;

		case 4: 
			Serial.println("INT 4 Triggered");
			val = 0;
			break;

		default:
	    	break;
	}
}

