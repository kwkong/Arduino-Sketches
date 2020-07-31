int array1[4] = {1,2,3};

void setup()
{
	Serial.begin(9600);

	arrayTest(array1);
}

void loop()
{

}

void arrayTest(int inArray[4])
{
	Serial.print(inArray[0]);
	Serial.print(inArray[1]);
	Serial.println(inArray[2]);
}