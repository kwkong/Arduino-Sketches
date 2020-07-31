#include <Wire.h>

#include <Adafruit_Colour_Sensor.h>



Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup()
{
    Serial.begin(9600);
    Serial.println("Color View Test!");

    pinMode(4, OUTPUT);

    digitalWrite(4, HIGH);
    delay(1000);

    if (tcs.begin())
    {
        Serial.println("Found sensor");
    }

    else
    {
        Serial.println("No TCS34725 found ... check your connections");
        while (1); // halt!
    }

    delay(1000);
    digitalWrite(4, LOW);
}

void loop()
{
    uint16_t red, green, blue, clear;
    uint16_t reds[10], greens[10], blues[10];
    uint16_t redSum, greenSum, blueSum;

    digitalWrite(4, HIGH);

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

    digitalWrite(4, LOW);

	red = redSum / 10;
	green = greenSum / 10;
	blue = blueSum / 10;

	//tcs.getRawData(& red, & green, & blue, &clear);

	Serial.print("Red: "); Serial.print(red);
	Serial.print("    Green: "); Serial.print(green);
	Serial.print("    Blue: "); Serial.println(blue);



    if ((red > 0) && (red < 200) && (green > 300) && (green < 1000) && (blue > 200) && (blue < 500))
    {
        Serial.println("Shit is green yo.");
    }

    else if ((red > 0) && (red < 200) && (green > 100) && (green < 300) && (blue > 300) && (blue < 800))
    {
        Serial.println("Shit is blue yo.");
    }

    else if ((red > 400) && (red < 1000) && (green > 100) && (green < 300) && (blue > 100) && (blue < 300))
    {
        Serial.println("Shit is red yo.");
    }

    else
    {
        Serial.println("idk what that shit is yo");
    }

    delay(300);
}
