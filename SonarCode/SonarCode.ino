//Initial Author: Bruce Allen
//Edited by: Kriston Kong
//Date Created: 23/07/09
//Date Edited: 21/1/17



//PWM pins for reading in the pulse width from the MaxSonar device.

const int pwmPin1 = 6;
const int pwnPin13 = 13;

long pulse1, inches1, cm1;

void setup()

{

  Serial.begin(9600);

  pinMode(pwmPin1, INPUT);
  pinMode(pwnPin13, OUTPUT);

}

void loop()
{
  //Used to read in the pulse that is being sent by the MaxSonar device.
  //Pulse Width representation with a scale factor of 147 uS per Inch.
  
  pulse1 = pulseIn(pwmPin1, HIGH);
  
  //147uS per inch
  inches1 = pulse1 / 147;

  //change inches to centimetres
  cm1 = inches1 * 2.54;

  Serial.print("Distance in inches: ");
  Serial.println(inches1);

  if (inches1 >= 17)
{
  digitalWrite(pwnPin13, HIGH);
}

if (inches1 <17)

{

  digitalWrite(pwnPin13, LOW);
}

 // delay(200);
}
