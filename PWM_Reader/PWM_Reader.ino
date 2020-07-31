//Initial Author: Bruce Allen
//Edited by: Kriston Kong
//Date Created: 23/07/09
//Date Edited: 21/1/17


#include <Servo.h>


Servo pwm1, pwm2, pwm3, pwm4, pwm5, pwm6;


int pwmOut1 = 2;
int pwmOut2 = 3;
int pwmOut3 = 4;
int pwmOut4 = 5;
int pwmOut5 = 6;
int pwmOut6 = 7;

const int pwmPin1 = 22;
const int pwmPin2 = 24;
const int pwmPin3 = 26;
const int pwmPin4 = 28;
const int pwmPin5 = 30;
const int pwmPin6 = 32;

long pulse1;
long pulse2;
long pulse3;
long pulse4;
long pulse5;
long pulse6;

void setup()

{
  Serial.begin(9600);
  pinMode(pwmPin1, INPUT);
  pinMode(pwmPin2, INPUT);
  pinMode(pwmPin3, INPUT);
  pinMode(pwmPin4, INPUT);
  pinMode(pwmPin5, INPUT);
  pinMode(pwmPin6, INPUT);

  
  pwm1.attach(pwmOut1); 
  pwm2.attach(pwmOut2);
  pwm3.attach(pwmOut3);
  pwm4.attach(pwmOut4);
  pwm5.attach(pwmOut5);
  pwm6.attach(pwmOut6);
}

void loop()

{
  //Used to read in the pulse that is being sent by the MaxSonar device.
  //Pulse Width representation with a scale factor of 147 uS per Inch.

  pwm1.write(1000); //ARD pin2 -> NAZE pin1 -> Roll
  pwm2.write(1000); //ARD pin3 -> NAZE pin2 -> Pitch
  pwm3.write(1000); //ARD pin4 -> NAZE pin3 -> Throttle
  pwm4.write(1000); //ARD pin5 -> NAZE pin4 -> Yaw
  pwm5.write(1000); //ARD pin6 -> NAZE pin5 -> Aux1
  pwm6.write(1000); //ARD pin7 -> NAZE pin6 -> Aux2
  
  pulse1 = pulseIn(pwmPin1, HIGH);
  pulse2 = pulseIn(pwmPin2, HIGH);
  pulse3 = pulseIn(pwmPin3, HIGH);
  pulse4 = pulseIn(pwmPin4, HIGH);
  pulse5 = pulseIn(pwmPin5, HIGH);
  pulse6 = pulseIn(pwmPin6, HIGH);
  

  Serial.print("PWM Value1: ");
  Serial.print(pulse1);
  Serial.println();
  Serial.print("PWM Value2: ");
  Serial.print(pulse2);
  Serial.println();
  Serial.print("PWM Value3: ");
  Serial.print(pulse3);
  Serial.println();
  Serial.print("PWM Value4: ");
  Serial.print(pulse4);
  Serial.println();
  Serial.print("PWM Value5: ");
  Serial.print(pulse5);
  Serial.println();
  Serial.print("PWM Value6: ");
  Serial.print(pulse6);
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  
 // delay(500);
}
