
#include "SevSeg.h"

#define averageSize 100
#define averageTime 10
#define averageDiff 1

//Create an instance of the object.
SevSeg myDisplay;

//Create global variables
unsigned long timer;
char myString[10];
int val = 0;
int oldVal = 0;
int valAvg = 0;
int averageBit = 0;
int vals[averageSize];
long valSum = 0;


void setup()
{

  int displayType = COMMON_CATHODE; //Your display is either common cathode or common anode


  //This pinout is for OpenSegment PCB layout
  //Declare what pins are connected to the digits
  int digit1 = A3; //Pin 12 on my 4 digit display
  int digit2 = A2; //Pin 9 on my 4 digit display
  int digit3 = A1; //Pin 8 on my 4 digit display
  int digit4 = A0; //Pin 6 on my 4 digit display

  //Declare what pins are connected to the segments
  int segA = 12; //Pin 11 on my 4 digit display
  int segB = 10; //Pin 7 on my 4 digit display
  int segC = 8; //Pin 4 on my 4 digit display
  int segD = 6; //Pin 2 on my 4 digit display
  int segE = 5; //Pin 1 on my 4 digit display
  int segF = 11; //Pin 10 on my 4 digit display
  int segG = 9; //Pin 5 on my 4 digit display
  int segDP= 7; //Pin 3 on my 4 digit display

  int numberOfDigits = 4; //Do you have a 1, 2 or 4 digit display?

  myDisplay.Begin(displayType, numberOfDigits, digit1, digit2, digit3, digit4, segA, segB, segC, segD, segE, segF, segG, segDP);
  
  myDisplay.SetBrightness(100); //Set the display to 100% brightness level

  Serial.begin(9600);
}

void loop()
{
	average();

	if((abs(oldVal - valAvg)) < 15)
	{
		valAvg = oldVal;
	}

	else 
	oldVal = valAvg;

	sprintf(myString, "%4d", valAvg);
	myDisplay.DisplayString(myString, 2);
	
}

void average()		//Startup average background value for sensor 1
{
	valSum = 0;

	for (int i=0; i<averageSize; i++)
	{
		vals[i] = analogRead(A6);

		vals[i] =  map(vals[i],0,900,0,3000);
		vals[i] *= 1.190;
		valSum += vals[i];
	}
	
	valAvg = valSum / averageSize;
}

