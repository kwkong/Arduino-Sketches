// Pulse Monitor Test Script


#define sensorPin 0
#define sqWavePin 1
#define outputPin 12
#define LEDpin 13

#define threshold 2 

int newVal[10];
int newVal_avg, oldVal_avg;
int countY;
int AvgY;
int newAvgY;
int oldAvgY;
int rawVal;

int sqWavePinVal;

void setup ()
{
      pinMode(sensorPin, INPUT);
      pinMode(outputPin, OUTPUT);
      pinMode(LEDpin, OUTPUT);
      Serial.begin (9600);
}
void loop ()
{
    //rawVal = analogRead(sensorPin);
   // Serial.print(rawVal);
    if(countY <= 10)
    {
      AvgY += analogRead(A0); 
      countY++;
    }
    if(countY >= 10)
    {
      newAvgY = AvgY /10;
      AvgY = 0;
      countY=0;
      oldAvgY = newAvgY;
    }
   // Serial.print("\t");
    Serial.print(newAvgY);
   if ((abs((newAvgY - oldAvgY))) >= threshold)
    {
        digitalWrite(LEDpin, HIGH);
        digitalWrite(outputPin, HIGH);
    }
    else
    {
        digitalWrite(LEDpin, LOW);
        digitalWrite(outputPin, LOW);
    }
    sqWavePinVal = digitalRead(sqWavePin);
    if(sqWavePinVal == 1);
      sqWavePinVal = newAvgY+100;
    if(sqWavePinVal == 0);
      sqWavePinVal = newAvgY-100;
    Serial.print("\t");
    Serial.println(analogRead(sqWavePinVal));
   
 
//    for (int i = 0; i < 10; i++)
//    {
//        newVal[i] = analogRead(0);
//     //   delay(5);
//        newVal_avg += newVal[i]; 
//    }
//    
//    newVal_avg = newVal_avg/10;
//
//    if ((abs((newVal_avg - oldVal_avg))) >= threshold)
//    {
//        digitalWrite(LEDpin, HIGH);
//        digitalWrite(outputPin, HIGH);
//    }
//    else
//    {
//        digitalWrite(LEDpin, LOW);
//        digitalWrite(outputPin, LOW);
//    }
//
//    newVal_avg = oldVal_avg;
//
//    Serial.print(analogRead(sensorPin));
//    Serial.print(newVal_avg);
//    
//    Serial.print(", ");
//    Serial.println(analogRead(sqWavePin));

}
