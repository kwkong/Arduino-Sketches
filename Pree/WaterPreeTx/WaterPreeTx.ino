#include <SoftwareSerial.h>

#define echoPin 11
#define trigPin 10

SoftwareSerial mySerial(2, 3);

int dist = 0;

void setup()
{
	Serial.begin(9600);
    mySerial.begin(38400);

	pinMode(echoPin, INPUT);
	pinMode(trigPin,OUTPUT);
}

void loop()
{
    //mySerial.print(num);
   // mySerial.print('s');
    // mySerial.write(getDist());
    // while(!(mySerial.available()));
    //   while(mySerial.available())
    //   {
    //     mySerial.read();
    //   }

    //delay(100);

    digitalWrite(trigPin, LOW);
    delayMicroseconds(100);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(100);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH, 10000);

    dist = duration/29;   //convert to cm
    dist = dist/2;


    // mySerial.write(dist);
    // mySerial.write('\n');
    mySerial.println(String(dist));
    Serial.println(String(dist));
    delay(100);
}

int getDist()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(100);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(100);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH, 10000);

  int dist = duration/29;   //convert to cm
  dist = dist/2;

  Serial.println(dist);
  

  return dist;
}

