#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX


void setup() 
{
	pinMode(7, OUTPUT);
	pinMode(8, OUTPUT);
	pinMode(13, OUTPUT);

	digitalWrite(7, LOW);
	digitalWrite(8, LOW);

	Serial.begin(9600);	
	mySerial.begin(9600);

	Serial.println("Init");

	delay(200);

	String message = "123456789";
	int messageLength = message.length() + 1;
	char messageArray[messageLength];
	message.toCharArray(messageArray, messageLength);
	int sendLength = messageLength + 2;
	uint8_t sendArray[sendLength];

	sendArray[0] = 0x05;
	sendArray[1] = 0x01;
	sendArray[2] = 0x15;

	for(int i=0; i<messageLength; i++)
	{
	    sendArray[i+3] = messageArray[i]; 
	}

	while(1)
	{
		mySerial.write(sendArray, sendLength);
		mySerial.println("");

		digitalWrite(13, (!digitalRead(13)));

		delay(1000);
	}

}

void loop()
{
	//uint8_t SendBuf[4] = {0x05, 0x01, 0x15, 0x0F};
	

}

// RET_STATUS SendMsg()
// {
//   RET_STATUS STATUS = RET_SUCCESS;

//   SwitchMode(MODE_0_NORMAL);

//   if(ReadAUX()!=HIGH)
//   {
//     return RET_NOT_IMPLEMENT;
//   }
//   delay(10);
//   if(ReadAUX()!=HIGH)
//   {
//     return RET_NOT_IMPLEMENT;
//   }

//   //TRSM_FP_MODE
//   //Send format : ADDH ADDL CHAN DATA_0 DATA_1 DATA_2 ...

//   String msg1 = "abcdefghijklmnop";
//   char charbuf[50];

//   msg1.toCharArray(charbuf,8);

//   uint8_t testChar1 = 'a';
//   uint8_t testChar2 = 'b';

// #ifdef Device_A
//   uint8_t SendBuf[4] = { DEVICE_B_ADDR_H, DEVICE_B_ADDR_L, 0x15, testChar1};	//for A
//   //Serial.println("dev A");
//   //uint8_t SendBuf[4] = { DEVICE_B_ADDR_H, DEVICE_B_ADDR_L, 0x1F, 0x17};
// #else
//   // uint8_t SendBuf[4] = { DEVICE_A_ADDR_H, DEVICE_A_ADDR_L, 0x15, random(0x81, 0xFF)};
//   uint8_t SendBuf[5] = { DEVICE_A_ADDR_H, DEVICE_A_ADDR_L, 0x15, charbuf[0], charbuf[1]};
//   //Serial.println("sending to dev B");
//   //uint8_t SendBuf[4] = { DEVICE_A_ADDR_H, DEVICE_A_ADDR_L, 0x1F, 0x17};	//for B
// #endif
//   mySerial.write(SendBuf, 5);

