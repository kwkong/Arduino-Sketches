// #include <SoftwareSerial.h>
// #define DEBUG true
// #include <stdio.h>  /* snprintf */
// 
// #include <MemoryFree.h>
#include <math.h>
// #define CIPSTART "AT+CIPSTART=\"TCP\",\"preelabs.com\",80\r\n"
// SoftwareSerial esp(2, 3);
#define RELAY_PIN 4
float currentRMS;
short relayState = 0;
short manualPin = 8;
short connectLED = 7;
short relayLED = 6;
int iMax;
int iMin = 1000;
int iMid;
int iDelta;
/*    current sensor variables  */
const int currentPin = A0; // ACS712 pin to Arduino A4 port
const unsigned long sampleTime = 100000UL; // sample over 100ms, it is an exact number of cycles for both 50Hz and 60Hz mains
const unsigned long numSamples = 250UL; // choose the number of samples to divide sampleTime exactly, but low enough for the ADC to keep up
const unsigned long sampleInterval = sampleTime/numSamples; // the sampling interval, must be longer than then ADC conversion time
short adc_zero;
// const int sampleCount = 100;
int voltages[400];
// unsigned long timeStamp[200];
void setup()
{
    Serial.begin(9600);
    // esp.begin(9600);
    pinMode(13, OUTPUT);
    pinMode(connectLED, OUTPUT);
    pinMode(relayLED, OUTPUT);
    digitalWrite(relayLED, LOW);
    pinMode(manualPin, INPUT_PULLUP);
    // pinMode(
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);
    relayState = 1;
    Serial.println("start");
    // adc_zero = determineVQ(currentPin);
    digitalWrite(RELAY_PIN, HIGH);
}

unsigned long intervalTime;
unsigned long startTime;
void loop()
{
    getVoltages();
    printVoltages();
    // Serial.println(iDelta);
    // iMax = 0;
    // iMin = 1000;
    // iMid= 0;
    // iDelta=0;
    // //startTime = millis();
    // // currentRMS =  roundf(readCurrent(currentPin)*100) / 100;
    // //  if(relayState == 1)
    // //    {
    // //      relayState = 0;
    // //      digitalWrite(RELAY_PIN, LOW);
    // //    }
    // //    else if(relayState == 0)
    // //    {
    // //      relayState = 1;
    // //      digitalWrite(RELAY_PIN, HIGH);
    // //    }
    // 
    // while(millis() <= startTime + 1000)
    // {
    // digitalWrite(RELAY_PIN, LOW);
    // //if(millis() >= intervalTime)
    // //  if(relayState == 1)
    // //    {
    // //      relayState = 0;
    // //      digitalWrite(RELAY_PIN, LOW);
    // //    }
    // //    else if(relayState == 0)
    // //    {
    // //      relayState = 1;
    // //      digitalWrite(RELAY_PIN, HIGH);
    // //    }
    // 
    // // startTime = millis();
    // //  currentRMS =  roundf(readCurrent(currentPin)*100) / 100;
    // 
    // //   getRawData();
    // // Serial.print("c = ");
    // //   Serial.println(currentRMS);
    // //Serial.println(analogRead(A0));
    // 
    // getVoltages();
    // ////  Serial.println("how u doin");
    // printVoltages();
    // // delay(500);
    // 
    // }
    // 
    // 
    // startTime = millis();
    // // while(millis() <= startTime + 15000)
    // while(1)
    // {
    // digitalWrite(RELAY_PIN, HIGH);
    // getVoltages();
    // ////  Serial.println("how u doin");
    // printVoltages();
    // }
    // startTime = millis();
    // while(millis() <= startTime + 1000)
    // {
    // digitalWrite(RELAY_PIN, LOW);
    // getVoltages();
    // ////  Serial.println("how u doin");
    // printVoltages();
    // }
    // startTime = millis();
    // while(millis() <= startTime + 15000)
    // {
    // digitalWrite(RELAY_PIN, HIGH);
    // getVoltages();
    // ////  Serial.println("how u doin");
    // printVoltages();
    // }
    // //  Serial.print("v = ");
    // //Serial.println("hey");
    // //  getVoltages();
    // ////  Serial.println("how u doin");
    // //  printVoltages();
    // //getFreq();
    // //  delay(5000);
}

void getVoltages()
{
    int count1 = 0;
    while (count1 <= 399)
    {
        // Serial.println("boom");
        // timeStamp[count1] = micros();
        voltages[count1] = analogRead(A0);
        if (iMax < voltages[count1])
        {
            iMax = voltages[count1];
        }
        if (iMin > voltages[count1])
        {
            iMin = voltages[count1];
        }
        iMid = iMax + iMin;
        iMid = iMid/2;
        iDelta = iMax - iMid;
        count1++;
    }
    // voltages[count1] = 580;
}

void printVoltages()
{
    // int count1 = 0;
    // while (count1 <= 399)
    // {
    //     // Serial.println("bang");
    //     // Serial.print(timeStamp[count1]);
    //     // Serial.print("\t");
    //     //Serial.print(voltages[count1]);


    //    // count1++;

    // }
    // Serial.print("I Max: ");
    // Serial.println(iMax);
    // 
    // Serial.print("I Min: ");
    // Serial.println(iMin);
    // 
    // Serial.print("I Mid: ");
    // Serial.println(iMid);
    // Serial.print("I Delta: ");
    // Serial.println(iDelta);

    //Serial.print(iMax);
    //Serial.print(',');
    Serial.println(iMid);
    //Serial.print(',');
    //Serial.println(iMin);

    iMax = 0;
    iMin = 1000;
    iMid = 0;
    iDelta = 0;
}

void getFreq()
{
    int state = 0;
    unsigned long endTime;
    unsigned long startTime;
    while (state != 10)
    {
        int rVoltage = analogRead(A1);
        if (rVoltage == 0)
            //looks for the first 0 to cancel first incomplete pulse
        {
            //startTime = micros();
            state = 1;
        }
        while (state == 1)
            //looks for rising edge of first pulse
        {
            rVoltage = analogRead(A1);
            if (rVoltage > 5)
                state = 2;
        }
        while (state == 2)
            //looks for first 0 after pulse to start clock
        {
            rVoltage = analogRead(A1);
            if (rVoltage == 0)
            {
                startTime = micros();
                state = 3;
            }
        }
        while (state == 3)
            //looks for first rising edge after pulse to be measured
        {
            rVoltage = analogRead(A1);
            if (rVoltage > 5)
            {
                // endTime = micros();
                state = 4;
            }
        }
        while (state == 4)
            //looks for second 0 to take period
        {
            rVoltage = analogRead(A1);
            if (rVoltage == 0)
            {
                endTime = micros();
                state = 5;
            }
        }
        while (state == 5)
        {
            float period = endTime - startTime;
            float freq = 1/period;
            freq = freq * 1000000;
            Serial.print("period: ");
            Serial.println(period);
            Serial.print("freq: ");
            Serial.println(freq);
            state = 10;
        }
    }
}

void getRawData(void)
{
    currentRMS = roundf(readCurrent(currentPin) * 100) / 100;
    // float currentRMS = roundf(readCurrent(currentPin)*100) / 100;
    // currentRMS = roundf(readCurrent(currentPin)*100) / 100;
    // currentRMS = 11.04;
    // -     delay(150);
    // -     Serial.print("Current = ");
    // -     Serial.println(currentRMS);
    // -     char bufferO[5];
    // snprintf(bufferO, 5, "%f", currentRMS);
    // sprintf(c,"%f", currentRMS);
    // -     ftoa(currentRMS, c, 2);
    // c = bufferO;
    // memcpy(c, bufferO, sizeof(c));
    // -     Serial.print("c = ");
    // -     Serial.println(c);
    // return currentRMS;
}

int determineVQ(int PIN)
{
    Serial.print("estimating avg. quiscent voltage:");
    long VQ = 0;
    // read 5000 samples to stabilise value
    for (int i = 0; i < 5000; i++)
    {
        VQ += analogRead(PIN);
        delay(1); // depends on sampling (on filter capacitor), can be 1/80000 (80kHz) max.
    }
    VQ /= 5000;
    Serial.print(map(VQ, 0, 1023, 0, 5000));
    Serial.println(" mV");
    return int(VQ);
}

float readCurrent(int PIN)
{
    // short adc_zero;
    unsigned long currentAcc = 0;
    unsigned int count = 0;
    unsigned long prevMicros = micros() - sampleInterval;
    while (count < numSamples)
    {
        if (micros() - prevMicros >= sampleInterval)
        {
            long adc_raw = analogRead(currentPin) - adc_zero;
            currentAcc += (unsigned long) (adc_raw * adc_raw);
            ++count;
            prevMicros += sampleInterval;
        }
    }
    float rms = sqrt((float) currentAcc/ (float) numSamples) * (50 / 1024.0); // '50' come from 5v/X=0.1v/1A of ACS712 0.1v per Amp spec on 20A version
    return rms;
    // Serial.println(rms);
}

float readVoltage(int PIN)
{
    // short adc_zero;
    unsigned long voltageAcc = 0;
    unsigned int count = 0;
    unsigned long prevMicros = micros() - sampleInterval;
    while (count < numSamples)
    {
        if (micros() - prevMicros >= sampleInterval)
        {
            long adc_raw = analogRead(currentPin) - adc_zero;
            voltageAcc += (unsigned long) (adc_raw * adc_raw);
            ++count;
            prevMicros += sampleInterval;
        }
    }
    float rms = sqrt((float) voltageAcc/ (float) numSamples) * (50 / 1024.0); // '50' come from 5v/X=0.1v/1A of ACS712 0.1v per Amp spec on 20A version
    return rms;
    // Serial.println(rms);
}
