#include <Wire.h>
#include <Adafruit_Colour_Sensor.h>

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup()
{
    Serial.begin(9600);
    Serial.println("Color View Test!");

    if (tcs.begin())
    {
        Serial.println("Found sensor");
    }

    else
    {
        Serial.println("No TCS34725 found ... check your connections");
        while (1); // halt!
    }
}

void loop()
{
    uint16_t red, green, blue, clear;
    uint16_t reds[10], greens[10], blues[10];
    uint16_t redSum, greenSum, blueSum;

    for (int i=0; i<10; i++)
    {
   		tcs.getRawData(& red, & green, & blue, &clear);

   		reds[i] = red;
   		greens[i] = green;
   		blues[i] = blue;

   		redSum += reds[i];
   		greenSum += greens[i];
   		blueSum += blues[i];

	}

	red = redSum / 10;
	green = greenSum / 10;
	blue = blueSum / 10;

	tcs.getRawData(& red, & green, & blue, &clear);

	Serial.print("Red: "); Serial.print(red);
	Serial.print("    Green: "); Serial.print(green);
	Serial.print("    Blue: "); Serial.println(blue);

    delay(300);
}
