
float voltageM, currentM, voltageC, currentC;

void setup() 
{
  Serial.begin(115200);
}

void loop() 
{
  serialCalibrate();
}

void serialCalibrate()
{
  Serial.println("Enter Voltage Gradient(m)");
  String inputVal = inString();
  voltageM = inputVal.toFloat();
  
  Serial.println("Enter Voltage intercept(C)");
  inputVal = inString();
  voltageC = inputVal.toFloat();

  Serial.println("Enter Current Gradient(m)");
  inputVal = inString();
  currentM = inputVal.toFloat();

  Serial.println("Enter Current Gradient(C)");
  inputVal = inString();
  currentC = inputVal.toFloat();
  
  Serial.print("voltageM: ");
  Serial.println(voltageM,6);
  Serial.print("voltageC: ");
  Serial.println(voltageC,6);
  Serial.print("currentM: ");
  Serial.println(currentM,6);
  Serial.print("currentC: ");
  Serial.println(currentC,6);
}

String inString()    //Waits for the user to send command via Serial
{
  boolean serialComplete = false;
  String inString = "";

  while (serialComplete == false)
  {
    while (Serial.available()) 
    {
      char inChar = (char)Serial.read();
      inString += inChar;

      if (inChar == '\n') 
      {
        serialComplete = true;
      }
    }
  }

  return inString;
}
