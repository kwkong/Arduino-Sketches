int val;
void setup()
{
  pinMode(A0, INPUT);
  Serial.begin(9600);
}
void loop()
{
  val = analogRead(A0);
  Serial.print("Value:");
  Serial.println(val);
  delay(300);
}
