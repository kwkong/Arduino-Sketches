int pot = 0;
int potOld = 0;
int rVal = 0;
int gVal = 0;
int bVal = 0;

String potString = "";


void setup()
{
	Serial.begin(9600);
}

void loop()
{
	//getPotVal();
	//Serial.println(potString);

	//Serial.println(analogRead(A7));

	pot = analogRead(analogRead(A7));

	pot = map(pot, 0, 1023, 0, 4096);

	potString =  String(pot, HEX);   

	Serial.println(potString); 

	delay(50);
}

char numToHex(int val)		//converts a decimal integer to hex char
{
	char hexOut = '0';

	val = map(val, 0, 9, 0, 15);

	switch (val) 
	{
	    case 0 : 
	      hexOut = '0';
	      break;

	    case 1 : 
	      hexOut = '1';
	      break;

	    case 2 : 
	      hexOut = '2';
	      break;

	    case 3 : 
	      hexOut = '3';
	      break;

	    case 4 : 
	      hexOut = '4';
	      break;

	    case 5 : 
	      hexOut = '5';
	      break;

	    case 6 : 
	      hexOut = '6';
	      break;

	    case 7 : 
	      hexOut = '7';
	      break;

	    case 8 : 
	      hexOut = '8';
	      break;

	    case 9 : 
	      hexOut = '9';
	      break;

	    case 10 : 
	      hexOut = 'A';
	      break;

	    case 11 : 
	      hexOut = 'B';
	      break;

	    case 12 : 
	      hexOut = 'C';
	      break;

	    case 13 : 
	      hexOut = 'D';
	      break;

	    case 14 : 
	      hexOut = 'E';
	      break;

	    case 15 : 
	      hexOut = 'F';
	      break;

	    default:
	    break;
	      // do something
	}

	return hexOut;
}

void getPotVal()
{
	pot = analogRead(A7);	//######insert pin

	// if (( pot < (potOld-10)) || (pot > (potOld+10)))	//only changes the value if the pot value changes significantly
	// {
		potOld = pot;
		pot = map(pot, 0, 1023, 0, 999);	//maps the pot analog range to 999 for hex RGB conversion 


		rVal = pot/100;
		gVal = pot/10; gVal = gVal%10;
		bVal = pot%100; bVal = bVal%10;		//gets each individual digit from pot value

		potString = numToHex(rVal);
		potString += numToHex(gVal);
		potString += numToHex(bVal);
	//}
}