#define analogPin1 26
#define analogPin2 27
#define ippsSize 10



int count = 0;
int ippAvgOld[10];
int ippAvg[10];
int ipps[ippsSize][10];
int ippCount[10]={0,0,0,0,0,0,0,0,0,0};

void setup()
{
  	pinMode(analogPin1,INPUT);
  	pinMode(analogPin2,INPUT);
	Serial.begin(115200);
}

void loop()
{
    Serial.print("ipp1: ");
    Serial.println(getVals(26,1));
    // Serial.print("     ipp2:");
    // Serial.println(getVals(27,2));
    // Serial.print("     ipp3: ");
    // Serial.println(getVals(27,3));    
}

int getVals(int analogPin, int sensorNum) 
{
    int ipp;
    int ippMax = 0;
    int ippMin = 4000;

    int ippSpike[3];
    int ippCalc = 0;

    for (int i = 0; i < 3; i++)
    {
        ippSpike[i] = getIpp(60,analogPin);
    }

    for(int i=0; i<3; i++)   //finds the biggest ipp in the array
    {
        if(ippSpike[i] > ippMax)
        {
            ippMax = ippSpike[i];
        }
    }

    for(int i=0; i<3; i++)   //collects the value that isn't the spike reading
    {
        if(ippSpike[i] < ippMax)
        {
            ippCalc = ippSpike[i];
            break;
        }
    }

    if(ippCalc == 0)
    {
        ippCalc = ippMax;
    }    

    ipps[ippCount[sensorNum]][sensorNum] = ippCalc;   //stores the "not peak" value in an array to average over a set number of loops for each unique getVals call       
    ippCount[sensorNum]++;

    if(ippCount[sensorNum] > (ippsSize -1))    //averages every ippsSize # of loops
    {       
        for(int i=0; i<ippsSize; i++)       //sums all the ipps for the average
        {
            ippAvg[sensorNum] += ipps[i][sensorNum];
        }

        ippAvg[sensorNum]/= ippsSize;  //calculates average

        if ((ippAvg[sensorNum] > (ippAvgOld[sensorNum] +5)) || (ippAvg[sensorNum] < (ippAvgOld[sensorNum] -5)))     //if the new average is outside of the threshold of the old average, set old average to new average
        {
            ippAvgOld[sensorNum] = ippAvg[sensorNum];
        }
        
        ippAvg[sensorNum]= 0;
        ippCount[sensorNum] =0;
    }

    ipp = ippAvgOld[sensorNum];
    return ipp;
}


int getIpp(int sampleNumber, int analogPin) 
{
    int iMax = 0;
    int iMin = 5000;
    int _ipp = 0;
    int currents[sampleNumber +2];


    for(int i=0; i<sampleNumber; i++)
    {
        currents[i] = analogRead(analogPin);

        if (iMax < currents[i]) 
        {
            iMax = currents[i];
            //iTime = micros();
        }

        if (iMin > currents[i]) 
        {
            iMin = currents[i];
        }

        delayMicroseconds(980);
    }    

    _ipp = iMax - iMin;

    return _ipp;
}

