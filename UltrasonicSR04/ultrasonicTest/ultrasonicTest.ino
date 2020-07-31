int echoPin = 3;
int trigPin = 2;

void setup() 
{
  // initialize serial communication:
  Serial.begin(115200);
  pinMode(echoPin, INPUT);
  pinMode(trigPin,OUTPUT);
}

void loop() 
{
	getDist();
	//Serial.println(durati);
	//delay(100);
}
  
void getDist()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(100);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(100);
  digitalWrite(trigPin, LOW);

  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  
  long duration = pulseIn(echoPin, LOW, 5000);

  //duration = duration/29/2;

  Serial.println(duration);
  //delay(100);

  //return duration / 29 / 2;
}
