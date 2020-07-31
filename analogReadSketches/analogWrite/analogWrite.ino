

String inputString = "";  
int val;

void setup()
{
	Serial.begin(9600);
	pinMode(9, OUTPUT);
}

void loop()
{
	Serial.println("Enter PWM Value");
	getSerial();
	val = inputString.toInt();
	Serial.println(val);
	analogWrite(9, val);
}

String getSerial()		//Waits for the user to send command via Serial
{
	boolean serialComplete = false;
	inputString = "";

	while (serialComplete == false)
	{
		while (Serial.available()) 
		{
			char inChar = (char)Serial.read();
			inputString += inChar;

			if (inChar == '\n') 
			{
				serialComplete = true;
			}
		}
	}

	return inputString;
}