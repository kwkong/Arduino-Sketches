

unsigned long startTime =0;

void setup()
{
  Serial.begin(115200);
  ledFadeInit(14,0,5000,8);
}

void loop()
{
  ledFade(0,10);
}

void ledFade(int pwmChannel,int fadeSpeed)
{
    for (int i = 0; i < 255; i+=5)
    {
        ledcWrite(pwmChannel, i);
        delay(fadeSpeed);
    }

    for (int i = 255; i > 0; i-=5)
    {
        ledcWrite(pwmChannel, i);
        delay(fadeSpeed);
    }
}

void ledFadeInit(int pwmPin, int pwmChannel, int pwmFreq, int pwmResolution)
{
    pinMode(pwmPin, OUTPUT);
    ledcSetup(pwmChannel, pwmFreq, pwmResolution);
    ledcAttachPin(pwmPin, pwmChannel);
}
