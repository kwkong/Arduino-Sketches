

const int echoPin = 7;
const int trigPin = 8;
const int redLed = 9;
const int grnLed = 10;

long duration;
long distance;

void setup()
{
    // initialize serial communication:
    Serial.begin(9600);
    pinMode(echoPin, INPUT);
    pinMode(trigPin, OUTPUT);
}

void loop()
{
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
    distance = duration * 0.007896;

    Serial.print("Duration: ");
    Serial.print(duration);
    Serial.print("           Distance: ");
    Serial.println(distance);

    if (distance >= 6)
    {
        digitalWrite(grnLed, HIGH);
        digitalWrite(redLed, LOW);
    }

    if (distance < 6)
    {
        digitalWrite(redLed, HIGH);
        digitalWrite(grnLed, LOW);
    }

}
