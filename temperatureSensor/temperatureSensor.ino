int degC;

void setup() {
  pinMode(A0, INPUT);
  Serial.begin(9600);

}

void loop() {
  Serial.print("Temperature (C): ");
  degC = analogRead(A0);
  degC = degC/5.3;
 
  Serial.println(degC);
  delay(200);

}
