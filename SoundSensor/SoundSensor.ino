int val;
int dVal;

void setup()
{
	Serial.begin(9600);
	pinMode(13, OUTPUT);	
}

void loop()
{
	val = analogRead(A0);
	Serial.print("Value is: ");
	Serial.print(val);
	//delay(50);

	val = digitalRead(3);
	Serial.print("       Digital Value is: ");
	Serial.println(dVal);
	//delay(50)
}
