int val = 0;
float volts = 0;


int state = 0;
unsigned long thisPulse = 0;
unsigned long lastPulse = 0;
float period = 0;
float freq = 0;


unsigned long highPulse = 0;
unsigned long lowPulse = 0;

void setup()
{
	Serial.begin(115200);	
	pinMode(A1, INPUT);
	pinMode(A0, INPUT);
	pinMode(13, OUTPUT);
	delay(2000);
}

void loop()
{
	question3_9();
}

void question1_5()
{
	Serial.println(digitalRead(A1));
	delay(100);	
}

void question1_7()
{
	Serial.println(analogRead(A0));
	delay(10);	
}

void question1_9()
{
	if(digitalRead(A1))
		digitalWrite(13, HIGH);

	else
		digitalWrite(13, LOW);
}

void question2_6()
{
	Serial.println(analogRead(A0));
	delay(100);	
}

void question2_9()
{
	volts = analogRead(A0);
	volts /= 249;
	Serial.print("Voltage: ");
	Serial.print(volts);
	Serial.println("V");
	delay(100);
}

void question3_9()
{
	Serial.print("Period: ");
	Serial.print(period);
	Serial.print(", Frequency: ");
	Serial.println(freq);
	delay(50);

	if ((digitalRead(A1)))
	{
		lastPulse = millis();

		while (digitalRead(A1))
			thisPulse = millis();

		period = float((thisPulse - lastPulse));
		freq = 1 / period;
		state = 1;
	}

	//while (!digitalRead(A1))

	if((digitalRead(A1) == 0) && (state == 1))
		state = 0;
}

