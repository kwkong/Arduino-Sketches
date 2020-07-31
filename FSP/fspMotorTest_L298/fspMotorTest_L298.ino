
const int L2 = 8;
const int L1 = 7;
const int R1 = 10;
const int R2 = 11;
const int ENA = 5;
const int ENB = 6;


void setup()
{

	Serial.begin(9600);

	pinMode(L1, OUTPUT);
	pinMode(L2, OUTPUT);
	pinMode(R1, OUTPUT);
	pinMode(R2, OUTPUT);
	pinMode(ENA, OUTPUT);
	pinMode(ENB, OUTPUT);

	digitalWrite(L1, HIGH);
	digitalWrite(L2, LOW);
	digitalWrite(R1, LOW);
	digitalWrite(R2, HIGH);

	analogWrite(ENA, 255);
	analogWrite(ENB, 255);

	delay(1000);

	digitalWrite(L1, LOW);
	digitalWrite(L2, LOW);
	digitalWrite(R1, LOW);
	digitalWrite(R2, LOW);

	analogWrite(ENA, 0);
	analogWrite(ENB, 0);

}

void loop()
{	
	
}

