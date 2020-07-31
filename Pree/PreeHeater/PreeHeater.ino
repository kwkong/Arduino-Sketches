//#include <lcd_I2C.h>
//#include <Streaming.h>

// PreeLabs Water Bath Temperature Control System
// Author: Kriston Kong, Yekini Wallen-Bryan
// Created: August 14, 2017
//Edited: 21/8/2017

#include <OneWire.h>
#include <LiquidCrystal_I2C.h>
//#include <lcd_I2C.h>


// ----------Pins----------//
const int DS18S20_Pin = 2; // DS18S20 Signal pin on digital 2
const int switch_pin = 3; // Toggle Switch
const int grnLED = 6;
const int bluLED = 5;
const int redLED = 4;
const int relay_pin = 7;
const int buzzer_pin = 8;
// ------------------------//


LiquidCrystal_I2C lcd(0x3F, 16, 2);
OneWire ds(DS18S20_Pin);


int onTemp = 95;
int tmp;
int switchStatus;
int heaterStatus;
int old_heaterStatus;
int old_switchStatus = 1;
char tmpstring[10];


void setup()
{
    Serial.begin(9600);
    lcd.begin();
    pinMode(switch_pin, INPUT_PULLUP);
    pinMode(redLED, OUTPUT);
    pinMode(bluLED, OUTPUT);
    pinMode(grnLED, OUTPUT);
    pinMode(relay_pin, OUTPUT);
    pinMode(buzzer_pin, OUTPUT);
    digitalWrite(grnLED, HIGH); // Green Status LED to indicate the MCU is on
    startup();
}

void loop()
{
    buttonPressCheck();
    //tempAdjust();
    controlTemp();
    probeError();
}

// ----------Functions----------//
void buttonPressCheck()
{
    if (digitalRead(switch_pin) == 0)
    {
        while (digitalRead(switch_pin) == 0);

        if (switchStatus == 1)
            switchStatus = 0;

        else
            switchStatus = 1;
    }
}

float getTemp()
{
    // returns the temperature from one DS18S20 in DEG Celsius
    byte data[12];
    byte addr[8];

    if (!ds.search(addr))
    {
        // no more sensors on chain, reset search
        ds.reset_search();
        return - 1000;
    }

    if (OneWire::crc8(addr, 7) != addr[7])
    {
        Serial.println("CRC is not valid!");
        return - 1000;
    }

    if (addr[0] != 0x10 && addr[0] != 0x28)
    {
        Serial.print("Device is not recognized");
        return - 1000;
    }

    ds.reset();
    ds.select(addr);
    ds.write(0x44, 1); // start conversion, with parasite power on at the end
    byte present = ds.reset();
    ds.select(addr);
    ds.write(0xBE); // Read Scratchpad

    for (int i = 0; i < 9; i++)
    {
        // we need 9 bytes
        data[i] = ds.read();
    }

    ds.reset_search();
    byte MSB = data[1];
    byte LSB = data[0];
    float tempRead = ((MSB << 8) | LSB); // using two's compliment
    float TemperatureSum = tempRead / 16;
    TemperatureSum = ((TemperatureSum * 18 + 5) /10 + 32);
    TemperatureSum = TemperatureSum - 32;
    TemperatureSum = TemperatureSum * 5;
    TemperatureSum = TemperatureSum / 9;

    return(TemperatureSum);
    // Deduct 32, then multiply by 5, then divide by 9
}

void controlTemp()
{
    float temperature = getTemp();
    tmp = (int) temperature;
    
    display_temp();
    display_onTemp();
    
    if (tmp < onTemp)
    {
        // TURN HEATER ON
        heaterStatus = 1;

        if (heaterStatus != old_heaterStatus)
        {
            old_heaterStatus = heaterStatus;

            longBeep();

            digitalWrite(redLED, HIGH);
            digitalWrite(bluLED, LOW);
            digitalWrite(relay_pin, HIGH);
        }

        digitalWrite(redLED, HIGH);
        digitalWrite(bluLED, LOW);
        digitalWrite(relay_pin, HIGH);
    }

    else
    {
        // TURN HEATER OFF
        heaterStatus = 0;

        if (heaterStatus != old_heaterStatus)
        {
            old_heaterStatus = heaterStatus;
    
            longBeep();

            digitalWrite(redLED, LOW);
            digitalWrite(bluLED, HIGH);
            digitalWrite(relay_pin, LOW);
        }
        
        digitalWrite(redLED, LOW);
        digitalWrite(bluLED, HIGH);
        digitalWrite(relay_pin, LOW);
    }
}

void tempAdjust()
{
    while (switchStatus == 1)
    {
        lcd.clear();

        if (digitalRead(3) == 0)
        {
            switchStatus = 0;
        }

        if (old_switchStatus != switchStatus)
            //Fast beep on mode switch
        {
            fastBeep();
            old_switchStatus = switchStatus;
        }

        if (heaterStatus == 0)
            digitalWrite(bluLED, !digitalRead(bluLED)); // Blink blue LED

        if (heaterStatus == 1)
            digitalWrite(redLED, !digitalRead(redLED));

        display_temp();

        onTemp = analogRead(A0);
        onTemp = 1023 - onTemp;
        onTemp = map(onTemp, 0, 1023, 0, 100); // Set adjustable temperature range to 0 ~ 100 degC

        display_setTemp();

        delay(300);
    }
}

void display_temp()
{
    if (tmp > 99)
    {
        lcd.setCursor(0, 0);
        lcd.print("Current Temp:");
        lcd.print(tmp);
    }
    else
    {
        lcd.setCursor(0, 0);
        lcd.print("Current Temp: ");
        lcd.print(tmp);
    }
}

void display_onTemp()
{
    if (onTemp > 99)
    {
        lcd.setCursor(0, 1);
        lcd.print("Desired Temp:");
        lcd.print(onTemp);
    }
    else
    {
        lcd.setCursor(0, 1);
        lcd.print("Desired Temp: ");
        lcd.print(onTemp);
    }
}

void display_setTemp()
{
    if (onTemp > 99)
    {
        lcd.setCursor(0, 1);
        lcd.print("Set Temp:");
        lcd.print(onTemp);
    }
    else
    {
        lcd.setCursor(0, 1);
        lcd.print("Set Temp: ");
        lcd.print(onTemp);
    }
}

void probeError()
{
    while (tmp < 0)
    {
        digitalWrite(redLED, LOW);
        digitalWrite(relay_pin, LOW);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Probe Not");
        lcd.setCursor(0, 1);
        lcd.print("Connected");

        delay(500);

        tmp = getTemp();
    }
}

void startup()
{
    lcd.setCursor(0, 0);
    lcd.print("    PreeLabs");
    lcd.setCursor(0, 1);
    lcd.print("  Temp Control");

    startupBeep();
    delay(3000);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   Please Set");
    lcd.setCursor(0, 1);
    lcd.print("  Desired Temp");
    lcd.clear();
}

// ----------Beeps----------//
void fastBeep()
{
    digitalWrite(buzzer_pin, HIGH);
    delay(50);
    digitalWrite(buzzer_pin, LOW);
}

void longBeep()
{
    digitalWrite(buzzer_pin, HIGH);
    delay(150);
    digitalWrite(buzzer_pin, LOW);
}

void startupBeep()
{
    fastBeep();
    delay(300);
    fastBeep();
    delay(100);
    fastBeep();
    delay(100);
    fastBeep();
}

void doubleBeep()
{
    fastBeep();
    delay(100);
    fastBeep();
}

// -------------------------//
