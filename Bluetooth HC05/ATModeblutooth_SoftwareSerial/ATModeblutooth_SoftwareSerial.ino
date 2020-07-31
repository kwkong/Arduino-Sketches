#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 2); // RX, TX


void setup() 
{
	// 	pinMode(8, OUTPUT);
	// pinMode(7, OUTPUT);
	// digitalWrite(7, LOW);
	// digitalWrite(8, LOW);


	Serial.begin(9600);	
	
	mySerial.begin(115200);

	Serial.println("Init");

    sendSMS();
}

void loop()
{
	if (mySerial.available())

	Serial.write(mySerial.read());

	if (Serial.available())

	mySerial.write(Serial.read());
}


void sendSMS() {
  // AT command to set mySerial to SMS mode
  mySerial.print("AT+CMGF=1\r"); 
  delay(2000);

  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  mySerial.println("AT + CMGS = "8765462023""); 
  delay(2000);
  
  // REPLACE WITH YOUR OWN SMS MESSAGE CONTENT
  mySerial.println("Send nudes"); 
  delay(2000);

  // End AT command with a ^Z, ASCII code 26
  int endchar = 26;
  mySerial.write(endchar); 
  delay(100);
  //mySerial.println();
  // Give module time to send SMS
  //delay(5000); 
}
