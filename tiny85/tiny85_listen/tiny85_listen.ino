

void setup() 
{

Serial.begin(9600);	

Serial2.begin(9600); //Baud rate dependent on module settings

}


void loop()
{

if (Serial2.available())

Serial.println(Serial2.read(), DEC);

}
