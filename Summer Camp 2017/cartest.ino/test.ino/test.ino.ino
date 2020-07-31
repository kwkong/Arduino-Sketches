//test.ino.ino

#define LEDpin 12
#define Gear 11
#define Kill 10
#define Ignition 9
#define Seatbelt 5
#define Occupancy 4
#define Motor 3
#define Buzzer 2

void setup() {

  Serial.begin(9600);
  
  pinMode(2, OUTPUT);           //Buzzer
  pinMode(3, OUTPUT);           //Motor
  pinMode(4, INPUT_PULLUP);     //Occupancy
  pinMode(5, INPUT_PULLUP);     //Seatbelt
  pinMode(9, INPUT_PULLUP);     //Ignition
  pinMode(10, INPUT_PULLUP);    //Kill Switch
  pinMode(11, INPUT_PULLUP);    //Gearshift 
  pinMode(12, OUTPUT);          //LED

}

void loop() {

//   digitalWrite(2, HIGH);
//   digitalWrite(3, HIGH);
//   delay(1000);
//   digitalWrite(2, LOW);
//   digitalWrite(3, LOW);
//   delay(1000);


//      Serial.print("Pin Status: ");
//      Serial.println(digitalRead(Seatbelt));
//      delay(100);
//
//      digitalWrite(Motor, HIGH);
//      delay(2000);
//      digitalWrite(Motor, LOW);
//      delay(2000);


      Serial.print("Gearshift: ");
      Serial.println(digitalRead(Gear));
      delay(500);

      
      Serial.print("Kill Switch: ");
      Serial.println(digitalRead(Kill));
      delay(500);

      
      Serial.print("Ignition: ");
      Serial.println(digitalRead(Ignition));
      delay(500);

      
      Serial.print("Occupancy: ");
      Serial.println(digitalRead(Occupancy));
      delay(500);

      
      Serial.print("Seatbelt: ");
      Serial.println(digitalRead(Seatbelt));
      delay(500);


}
