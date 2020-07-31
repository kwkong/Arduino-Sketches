

const int triggerPin = 23;
const int echoPin = 25;

long dist;


void setup()
{
	Serial3.begin(38400);
	Serial.begin(9600);
	
	pinMode(triggerPin, OUTPUT);
}

void loop()
{
	digitalWrite(triggerPin, LOW);
	delay(10);
	digitalWrite(triggerPin, HIGH);
	delay(10);
	digitalWrite(triggerPin, LOW);

	dist = pulseIn(echoPin, HIGH);

	dist = dist*0.0175;

	Serial3.print("Distance (cm): ");
	Serial3.println(dist);	
}

