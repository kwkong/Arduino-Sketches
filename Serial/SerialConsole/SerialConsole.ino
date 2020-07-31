/*
AT+UART   	Baud rate of module

AT+ADDR		Address of module
				

AT+BIND		Address to bind to on startup
			Entered value must be comma separated, not colon

AT+CMODE	Set/query CMODE
			CMODE=1 makes the master pair to any slave in range

AT+ROLE		Set/query role
			AT+ROLE=1 for master, AT+ROLE=0 for slave
*/

#define bluSerial Serial2


void setup() {

Serial.begin(115200);

Serial.println("Enter AT commands:");

bluSerial.begin(9600);

}

void loop()

{

if (bluSerial.available())

Serial.write(bluSerial.read());

if (Serial.available())

bluSerial.write(Serial.read());

}


