
const int L2 = 6;
const int L1 = 5;
const int R2 = 3;
const int R1 = 4;




void setup()
{

	pinMode(13, OUTPUT);
	Serial.begin(9600);

	pinMode(L1, OUTPUT);
	pinMode(L2, OUTPUT);
	pinMode(R1, OUTPUT);
	pinMode(R2, OUTPUT);

	analogWrite(L1, 0);
	analogWrite(R1, 100);

	digitalWrite(13, HIGH);

	analogWrite(L2, 100);
	analogWrite(R2, 0);

	delay(1000);

	digitalWrite(13, LOW);

	analogWrite(L1, 0);
	analogWrite(R1, 0);

	analogWrite(L2, 0);
	analogWrite(R2, 0);


	// digitalWrite(L2, HIGH);
	// digitalWrite(R1, HIGH);
	// digitalWrite(L1, LOW);
	// digitalWrite(R2, LOW);
}

void loop()
{	
	
}

