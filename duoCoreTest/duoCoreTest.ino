/*
 * This blinks two LEDs independently and not synchronized. Both have other blink frequencies. 
 * The blink sketches run in two tasks and on two cores.
 */


TaskHandle_t Task1, Task2;

String inputString= "";
boolean stringComplete = false;


void codeForTask1( void * parameter )
{
  for (;;) 
  {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
  	}
	}
  }
}

void codeForTask2( void * parameter )
{
  if (stringComplete) {
    Serial.println(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

// the setup function runs once when you press reset or power the board
void setup() 
{
	Serial.begin(115200);
	// initialize digital pin LED_BUILTIN as an output.


	xTaskCreatePinnedToCore(
		codeForTask1,
		"led1Task",
		1000,
		NULL,
		1,
		&Task1,
		0);

	delay(500);  // needed to start-up task1

	xTaskCreatePinnedToCore(
		codeForTask2,
		"led2Task",
		1000,
		NULL,
		1,
		&Task2,
		1);

}

void loop() 
{
  delay(1000);
}