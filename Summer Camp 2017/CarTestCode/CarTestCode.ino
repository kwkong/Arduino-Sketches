int seatbeltLED = 12;
int gearShift = 11;
int killSwitch = 10;
int ignition = 9;
int alarm = 2;
int motor = 3;
int occSensor = 4;
int seatBelt = 5;

void setup() {
  // put your setup code here, to run once:

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(12, OUTPUT); 
  pinMode(11, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP); 
  pinMode(5, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP); 

  Serial.begin(9600);

}

void loop() {

//  digitalWrite(2, HIGH);
//  digitalWrite(3, HIGH);
//  delay(1000);
//  digitalWrite(2, LOW);
//  digitalWrite(3, LOW);
//  delay(1000);

//    digitalWrite(12, HIGH);
//    delay(1000);
//    digitalWrite(12, LOW);
//    delay(1000);

      //digitalRead(11);
      
      Serial.print("Gear Shift: ");
      Serial.println(digitalRead(11));
      Serial.print("Kill Switch: ");
      Serial.println(digitalRead(10));
      Serial.print("Ignition: ");
      Serial.println(digitalRead(9)); 
      Serial.print("Driver Occupancy: ");
      Serial.println(digitalRead(5));
      Serial.print("Seatbelt: ");
      Serial.println(digitalRead(4));
      delay(1000);


      
/*
      if(digitalRead(11) == 0)
      {
        digitalWrite(2,HIGH);
      }
      else
      {
        digitalWrite(2,LOW); 
      }
      delay(500);
*/
}
