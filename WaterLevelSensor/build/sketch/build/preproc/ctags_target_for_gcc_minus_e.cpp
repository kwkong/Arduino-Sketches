# 1 "c:\\Arduino\\My Sketches\\WaterLevelSensor\\WaterLevelSensor.ino"
# 1 "c:\\Arduino\\My Sketches\\WaterLevelSensor\\WaterLevelSensor.ino"
# 2 "c:\\Arduino\\My Sketches\\WaterLevelSensor\\WaterLevelSensor.ino" 2
# 3 "c:\\Arduino\\My Sketches\\WaterLevelSensor\\WaterLevelSensor.ino" 2




LiquidCrystal_I2C lcd(0x27, 16, 2);

int tankHeightMin = 130;
int tankHeightMax = 20;
int tankVol = 1600;

int maxDepth = 30;

float barCount = 0;
float barCountOld = 1;
int dist = 0;
int vol = 0;

void setup()
{
 pinMode(11, 0x0);
 pinMode(12,0x1);

 Serial.begin(9600);
    Serial.println("init");
 init_LCD();

}

void loop()
{
 getDist();
 printDisplay();
 delay(10000);
    Serial.println("loop");

}

void printDisplay()
{
    lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Volume:");

 lcd.setCursor(0,1);
 lcd.print('E');

 lcd.setCursor(15,1);
 lcd.print('F');

 barCount = map(dist, tankHeightMin, tankHeightMax, 0, 12);

 if (barCount > 12)
 {
  barCount = 12;
 }

 if (barCount != barCountOld)
 {
  printBar(barCount);
  printVol();
  barCountOld = barCount;
 }
}

void printVol()
{
 vol = barCount*133;
 Serial.println(vol);

 lcd.setCursor(8,0);

 if ((vol/1000) <= 0)
 {
  lcd.print(" ");
  lcd.setCursor(9,0);
 }

 lcd.print(vol);
 lcd.setCursor(13,0);
 lcd.print("gal");
}

void printBar(int barNum)
{
 int barDiff = 0;

 for(int i=0;i<barNum;i++)
 {
  lcd.setCursor((i+2),1);
  lcd.write(255);
 }

 delay(100);

 barDiff = 12 - barNum;

 for(int i=0;i<barDiff;i++)
 {
  lcd.setCursor((14-i),1);
  lcd.print(" ");
 }

}

void init_LCD()
{
 lcd.begin();
 lcd.backlight();
 lcd.print("System Start");
 lcd.setCursor(0,1);
 lcd.print("Water Monitor");
 delay(2000);
 lcd.clear();
}

void getDist()
{
    digitalWrite(12, 0x0);
    delayMicroseconds(100);
    digitalWrite(12, 0x1);
    delayMicroseconds(100);
    digitalWrite(12, 0x0);

    long duration = pulseIn(11, 0x1, 10000);

    dist = duration/29; //convert to cm
    dist = dist/2;

    // Serial.println(dist);

    delay(100);
}
