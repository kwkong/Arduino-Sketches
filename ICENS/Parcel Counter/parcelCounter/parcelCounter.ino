#define signalPin 2
#define ackPin 7

int val;

String inputString = "";         
boolean stringComplete = false; 


void setup()
{
	Serial.begin(9600);

	pinMode(signalPin, INPUT);
	pinMode(ackPin, OUTPUT);

	attachInterrupt(digitalPinToInterrupt(signalPin), detect, RISING);

	inputString.reserve(200);

	//detectTest();
}

void loop()
{
	//getCommand();
	sendCommand();
}


void detect()
{
	val = 1;
	Serial.println("Sample Detected");
}

void sendCommand()
{
	if (stringComplete) 
	{
		//Serial.println(inputString);
		
		stringComplete = false;

		if (inputString.charAt(0) == '1')
		{
			digitalWrite(ackPin, HIGH);
			Serial.println("Detector Enabled");
		}

		if (inputString.charAt(0) == '0')
		{
			digitalWrite(ackPin, LOW);
			Serial.println("Detector Disabled");
		}

		if (inputString.charAt(0) == 'k')
		{
			digitalWrite(ackPin, LOW);
			Serial.println("Enter Number");
		}

		inputString = "";
		
	}
}

// void sendCommand()
// {
// 	if (inputString.charAt(0) == '1')
// 	digitalWrite(ackPin, HIGH);

// 	if (inputString.charAt(0) == '0')
// 	digitalWrite(ackPin, LOW);
// }

void serialEvent() 
{
	while (Serial.available()) 
	{
		char inChar = (char)Serial.read();
		inputString += inChar;

		if (inChar == '\n') 
		{
			stringComplete = true;
		}
	}
}



// val = response.indexOf('*');

// if ((response.charAt(val) == '*') && (response.charAt(val+1) == 'o') && (response.charAt(val+2) == 'n'))

