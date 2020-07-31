int Ain1 = 3;
int Ain2 = 5;
int Bin1 = 6;
int Bin2 = 9;


void setup() {

pinMode(Ain1, OUTPUT);
pinMode(Ain2, OUTPUT);
pinMode(Bin1, OUTPUT);
pinMode(Bin2, OUTPUT);

  

}

void loop() {

      moveForward(100);
      delay(1000);
      moveStop();

}


void moveForward(int SPEED)
{
  analogWrite(Ain1, SPEED);
  analogWrite(Ain2, 0);
  analogWrite(Bin1, SPEED);
  analogWrite(Bin2, 0);
}

void moveStop()
{
  analogWrite(Ain1, 0);
  analogWrite(Ain2, 0);
  analogWrite(Bin1, 0);
  analogWrite(Bin2, 0);
}

