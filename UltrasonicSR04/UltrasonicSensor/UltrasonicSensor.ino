/* Ping))) Sensor

   This sketch reads a PING))) ultrasonic rangefinder and returns the
   distance to the closest object in range. To do this, it sends a pulse
   to the sensor to initiate a reading, then listens for a pulse
   to return.  The length of the returning pulse is proportional to
   the distance of the object from the sensor.

 */

// this constant won't change.  It's the pin number
// of the sensor's output:
const int echoPin = 24;
const int trigPin = 22;
long duration, cm;

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  pinMode(echoPin, INPUT);
  pinMode(trigPin,OUTPUT);
}

void loop() {
  

  // The Echo is triggered by a HIGH pulse of 15 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:

  digitalWrite(trigPin, LOW);
  delayMicroseconds(15);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(15);
  digitalWrite(trigPin, LOW);

  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  
  duration = pulseIn(echoPin, HIGH);

  // convert the time into a distance

  cm = microsecondsToCentimeters(duration);

  Serial.print(cm);
  Serial.println("cm");
  delay(100);
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
