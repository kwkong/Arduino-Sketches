
float pressure1, pressure2 = 0;


void getPressure()
{
    pressure1 = analogRead(A0);//-229;
    pressure1 = ((pressure1*0.0049)*1000)/9.8;

    pressure2 = analogRead(A1);//-237;
    pressure2 = ((pressure2*0.0049)*1000)/9.8;

    //Serial.print("P1: ");
    Serial.print(pressure1);
    Serial.print("\t");

    //Serial.print("P2: ");
    Serial.print(pressure2);
    Serial.print("\t");

    //Serial.print("delta: ");
    Serial.print((pressure1 - pressure2));
    Serial.print("\t");

    Serial.println(" ");
}


void setup() {
  Serial.begin(115200);

}

void loop() 
{

  getPressure();
  delay(10);
}
