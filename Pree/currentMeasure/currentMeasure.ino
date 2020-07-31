//#include <SoftwareSerial.h>
//#define DEBUG true
//#include <stdio.h>  /* snprintf */
//
//#include <MemoryFree.h>
//#include <math.h>

//#define CIPSTART "AT+CIPSTART=\"TCP\",\"preelabs.com\",80\r\n"
//SoftwareSerial esp(2, 3);


#define CURRENT_SENSOR
#define RELAY_PIN 10

float currentRMS;

short relayState = 0;


short manualPin = 8;

short connectLED = 7;
short relayLED = 6;



/*    current sensor variables  */
//const int currentPin = A0;      // ACS712 pin to Arduino A4 port
//const unsigned long sampleTime = 100000UL;                           // sample over 100ms, it is an exact number of cycles for both 50Hz and 60Hz mains
//const unsigned long numSamples = 250UL;                               // choose the number of samples to divide sampleTime exactly, but low enough for the ADC to keep up
//const unsigned long sampleInterval = sampleTime/numSamples;  // the sampling interval, must be longer than then ADC conversion time
//
//short adc_zero; 

//const int sampleCount = 100;
int counter[400];
int currents[200];
int voltages[200];

void setup() {
 
    Serial.begin(9600);
//    esp.begin(9600);
    pinMode(13,OUTPUT);
    pinMode(connectLED,OUTPUT);
    pinMode(relayLED,OUTPUT);
    digitalWrite(relayLED, LOW);
    pinMode(manualPin,INPUT_PULLUP);
    //pinMode(
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("start");

}

void loop() {
//   currentRMS =  roundf(readCurrent(currentPin)*100) / 100;
 // Serial.print("c = ");
//  Serial.println(currentRMS);
//  Serial.print("v = ");
//Serial.println("hey");
  //getVoltages();
  //getCurrent();
   getBoth();
  //printVoltages();
  //printCounter();
  printBoth();
//getFreq();
 // delay(1000);
  

}
void getBoth()
{
  int count1 = 0;
  while(count1 <= 199)
  {
  //  Serial.println("boom");
    currents[count1] = analogRead(A0);
    voltages[count1] = analogRead(A1);
    count1++;
  }
}
void getCurrent()
{
  int count1 = 0;
  while(count1 <= 399)
  {
  //  Serial.println("boom");
    counter[count1] = analogRead(A0);
    count1++;
  }
}
void printCounter()
{
  int count1 = 0;
  while(count1 <= 399)
  {
   // Serial.println("bang");
    Serial.println(counter[count1]);
    count1++;
  }
}
void printBoth()
{
  int count1 = 0;
  int buff;
  while(count1 <= 199)
  {
   // Serial.println("bang");
   buff = currents[count1];
   
    //Serial.print(currents[count1]);
    Serial.print(buff);
    Serial.print(",");
    buff = voltages[count1];
    Serial.println(buff);
   // Serial.println("4");
    //Serial.println(voltages[count1]);
    
    count1++;
  }
}
void getVoltages()
{
  int count1 = 0;
  while(count1 <= 399)
  {
  //  Serial.println("boom");
    counter[count1] = analogRead(A1);
    count1++;
  }
}

void getFreq()
{
  int state = 0;
  unsigned long endTime;
  unsigned long startTime;
  while(state != 10)
  {
    int rVoltage = analogRead(A1);
    if(rVoltage == 0)              //looks for the first 0 to cancel first incomplete pulse
    {
     //startTime = micros();
     state = 1;
    }
    while(state == 1)             //looks for rising edge of first pulse
    {
      rVoltage = analogRead(A1);
      if(rVoltage > 5)     
      state = 2;
    }
     while( state == 2)          //looks for first 0 after pulse to start clock
     {
        rVoltage = analogRead(A1);
        if(rVoltage == 0)
        {
          startTime = micros();
          state = 3;    
        }
     }
     while(state == 3)         //looks for first rising edge after pulse to be measured
      {
        rVoltage = analogRead(A1);
        if(rVoltage > 5)
          {
            //endTime = micros();
            state = 4;         
          }
      }
      while(state == 4)       //looks for second 0 to take period
      {
        rVoltage = analogRead(A1);
        if(rVoltage == 0)
          {
            endTime = micros();
            state = 5;         
          }
      }
      while(state == 5)
      {
       float period = endTime - startTime;
       float freq = 1/period;
       freq = freq*1000000;
       Serial.print("period: ");
       Serial.println(period);
       Serial.print("freq: ");
       Serial.println(freq);
       state = 10;
      }
    }
}
//void getRawData(void)
//{
//      currentRMS =  roundf(readCurrent(currentPin)*100) / 100;
//    // float currentRMS = roundf(readCurrent(currentPin)*100) / 100;
//     //currentRMS = roundf(readCurrent(currentPin)*100) / 100;
//    // currentRMS = 11.04;
////-     delay(150);
////-     Serial.print("Current = ");
////-     Serial.println(currentRMS);
//
////-     char bufferO[5];
//
//     
//     //snprintf(bufferO, 5, "%f", currentRMS);
//    // sprintf(c,"%f", currentRMS); 
////-     ftoa(currentRMS, c, 2);
//    // c = bufferO;
//     //memcpy(c, bufferO, sizeof(c));
////-     Serial.print("c = ");
////-     Serial.println(c);
//
//    
//     //return currentRMS;
//
//
//}
//int determineVQ(int PIN) 
//{
//  Serial.print("estimating avg. quiscent voltage:");
//  long VQ = 0;
//  //read 5000 samples to stabilise value
//  for (int i=0; i<5000; i++) {
//    VQ += analogRead(PIN);
//    delay(1);//depends on sampling (on filter capacitor), can be 1/80000 (80kHz) max.
//  }
//  VQ /= 5000;
//  Serial.print(map(VQ, 0, 1023, 0, 5000));Serial.println(" mV");
//  return int(VQ);
//}
//
//float readCurrent(int PIN)
//{
//  //short adc_zero;  
//  unsigned long currentAcc = 0;
//  unsigned int count = 0;
//  unsigned long prevMicros = micros() - sampleInterval ;
//  while (count < numSamples)
//  {
//    if (micros() - prevMicros >= sampleInterval)
//    {
//      long adc_raw = analogRead(currentPin) - adc_zero;
//      currentAcc += (unsigned long)(adc_raw * adc_raw);
//      ++count;
//      prevMicros += sampleInterval;
//    }
//  }
//  float rms = sqrt((float)currentAcc/(float)numSamples) * (50 / 1024.0);   //'50' come from 5v/X=0.1v/1A of ACS712 0.1v per Amp spec on 20A version
//  return rms;
//  //Serial.println(rms);
//}
//float readVoltage(int PIN)
//{
//  //short adc_zero;  
//  unsigned long voltageAcc = 0;
//  unsigned int count = 0;
//  unsigned long prevMicros = micros() - sampleInterval ;
//  while (count < numSamples)
//  {
//    if (micros() - prevMicros >= sampleInterval)
//    {
//      long adc_raw = analogRead(currentPin) - adc_zero;
//      voltageAcc += (unsigned long)(adc_raw * adc_raw);
//      ++count;
//      prevMicros += sampleInterval;
//    }
//  }
//  float rms = sqrt((float)voltageAcc/(float)numSamples) * (50 / 1024.0);   //'50' come from 5v/X=0.1v/1A of ACS712 0.1v per Amp spec on 20A version
//  return rms;
//  //Serial.println(rms);
//}