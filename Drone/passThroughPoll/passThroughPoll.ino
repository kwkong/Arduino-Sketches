#include <Servo.h>

int rollPin = 3, throttlePin = 6, pitchPin = 5, yawPin=9,auxPin1=10,auxPin2=11;
Servo roll,throttle,pitch,yaw,aux1,aux2;
int count = 0;

  const int pwmPin1 = 50;
  const int pwmPin2 = 51;
  const int pwmPin3 = 52;
  const int pwmPin4 = 53;

  long pulse1[2];
  long pulse2[2];
  long pulse3[2];
  long pulse4[2];

  int tol = 10;




void setup() {

  pinMode(rollPin,OUTPUT);
  roll.attach(rollPin);
  
  pinMode(throttlePin,OUTPUT);
  throttle.attach(throttlePin);

  pinMode(pitchPin,OUTPUT);
  pitch.attach(pitchPin);
  
  pinMode(yawPin,OUTPUT);
  yaw.attach(yawPin);
  
  pinMode(auxPin1,OUTPUT);
  aux1.attach(auxPin1);

  pinMode(auxPin2,OUTPUT);
  aux2.attach(auxPin2);

  Serial.begin(9600);
  pinMode(pwmPin1, INPUT);
  pinMode(pwmPin2, INPUT);
  pinMode(pwmPin3, INPUT);
  pinMode(pwmPin4, INPUT);

}


void loop() {

  pulse1[1] = pulseIn(pwmPin1, HIGH);
  pulse2[1] = pulseIn(pwmPin2, HIGH);
  pulse3[1] = pulseIn(pwmPin3, HIGH);
  pulse4[1] = pulseIn(pwmPin4, HIGH);

  Serial.print("PWM Value1: ");
  Serial.print(pulse1[1]);
  Serial.println();
  Serial.print("PWM Value2: ");
  Serial.print(pulse2[1]);
  Serial.println();
  Serial.print("PWM Value3: ");
  Serial.print(pulse3[1]);
  Serial.println();
  Serial.print("PWM Value4: ");
  Serial.print(pulse4[1]);
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();

  //delay(50);

  pulse1[2] = pulseIn(pwmPin1, HIGH);
  pulse2[2] = pulseIn(pwmPin2, HIGH);
  pulse3[2] = pulseIn(pwmPin3, HIGH);
  pulse4[2] = pulseIn(pwmPin4, HIGH);

//   roll.write(pulse1[1]);
//   yaw.write(pulse2[1]);
//   pitch.write(pulse3[1]);
//   throttle.write(pulse4[1]);

  if ((((pulse1[2])-(pulse1[1])) >= tol) || (((pulse1[2])-(pulse1[1])) <= (-tol)))
  {
    roll.write(pulse1[2]);
  }
  
  if ((((pulse2[2])-(pulse2[1])) >= tol) || (((pulse2[2])-(pulse2[1])) <= (-tol))) 
  {
    yaw.write(pulse2[2]);
  }
  
  if ((((pulse3[2])-(pulse3[1])) >= tol) || (((pulse3[2])-(pulse3[1])) <= (-tol)))
  {
    pitch.write(pulse3[2]);
  }
  
  if ((((pulse4[2])-(pulse4[1])) >= tol) || (((pulse4[2])-(pulse4[1])) <= (-tol)))
  {
    throttle.write(pulse4[2]);
  }

}

