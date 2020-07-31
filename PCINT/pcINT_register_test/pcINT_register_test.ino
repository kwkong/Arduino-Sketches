#include <PinChangeInt.h>


#define pcIntPin 3

volatile int val = 0;

void pcIntVect() 
{
  val = 1;
}

void setup() 
{
  pinMode(pcIntPin, INPUT_PULLUP);
  PCintPort::attachInterrupt(pcIntPin, pcIntVect, FALLING);
  
  Serial.begin(9600);

  Serial.println("Ready");
  while (val == 0);
  Serial.println("TRIGGERED");
}

void loop() 
{

}

