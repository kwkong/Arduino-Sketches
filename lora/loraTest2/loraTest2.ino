#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX


void setup() 
{
	pinMode(7, OUTPUT);
	pinMode(8, OUTPUT);

	digitalWrite(7, HIGH);
	digitalWrite(8, HIGH);

	Serial.begin(9600);	

	mySerial.begin(9600);

	Serial.println("Init");

	delay(200);

	mySerial.write(0xC1);
	mySerial.write(0xC1);
	mySerial.write(0xC1);	
}

void loop()
{
	if (mySerial.available())
	{
		int inByte = mySerial.read();

		if (inByte < 0x10)
		Serial.print("0x0");

		else
		Serial.print("0x");

		Serial.print((inByte),HEX);
		Serial.print(" ");
	}
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

