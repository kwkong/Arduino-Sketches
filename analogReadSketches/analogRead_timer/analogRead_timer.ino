int val;
int val2;

unsigned long time;

String inputString = "";
boolean serialComplete = false;

void setup()
{

	pinMode(A1,INPUT);
	Serial.begin(9600);

	// time = micros();

	// while (Serial.available()) 
	// {
	// 	char inChar = (char)Serial.read();
	// 	inputString += inChar;

	// 	if (inChar == '\n') 
	// 	{
	// 		Serial.println(inputString);
	// 		serialComplete = true;
	// 	}
	// }

	// val = analogRead(A1);
	// if (val > 500)
	// val2 = 1;

	// val = analogRead(A1);
	// if (val > 500)
	// val2 = 1;

	// val = analogRead(A1);
	// if (val > 500)
	// val2 = 1;

	// val = analogRead(A1);
	// if (val > 500)
	// val2 = 1;

	// val = analogRead(A1);
	// if (val > 500)
	// val2 = 1;

	// val = analogRead(A1);
	// if (val > 500)
	// val2 = 1;

	// val = analogRead(A1);
	// if (val > 500)
	// val2 = 1;

	// val = analogRead(A1);
	// if (val > 500)
	// val2 = 1;

	// time = micros() - time;

	// Serial.println(time);
}

void loop()
{
	time = micros();

	while (Serial.available()) 
	{
		char inChar = (char)Serial.read();
		inputString += inChar;

		if (inChar == '\n') 
		{
			Serial.println(inputString);
			serialComplete = true;
		}
	}

	val = analogRead(A1);
	if (val > 500)
	val2 = 1;

	val = analogRead(A2);
	if (val > 500)
	val2 = 1;

	val = analogRead(A3);
	if (val > 500)
	val2 = 1;

	val = analogRead(A4);
	if (val > 500)
	val2 = 1;

	val = analogRead(A5);
	if (val > 500)
	val2 = 1;

	val = analogRead(A6);
	if (val > 500)
	val2 = 1;

	val = analogRead(A7);
	if (val > 500)
	val2 = 1;

	val = analogRead(A0);
	if (val > 500)
	val2 = 1;

	time = micros() - time;

	Serial.println(time);

	delay(4000);
}	

