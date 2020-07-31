#include <IRremote.h>
#include <Servo.h>
 
#define RECV_PIN  12        //Infrared signal receiving pin
#define LED       13        //define LED pin
#define L         16720605
#define R 		  16761405
#define F         16736925
#define B         16754775
#define S         16712445
#define UNKNOWN_L 1386468383

/*define channel enable output pins*/
#define ENA 5	  // Left  wheel speed
#define ENB 6	  // Right wheel speed
/*define logic control output pins*/
#define IN1 7	  // Left  wheel forward
#define IN2 8	  // Left  wheel reverse
#define IN3 9	  // Right wheel reverse
#define IN4 11	// Right wheel forward
#define carSpeed 100	// initial speed of car >=0 to <=255
 
bool state = LOW;           //define default input mode
unsigned long val; 

 
IRrecv irrecv(RECV_PIN);   //initialization
decode_results results;   //Define structure type

Servo myServo;
 
void stateChange() 
{
  state = !state;          
  digitalWrite(LED, state);
}
 
void setup() 
{
	pinMode(LED, OUTPUT); //initialize LED as an output
	Serial.begin(9600);  // debug output at 9600 baud
	irrecv.enableIRIn();  // Start receiving

	myServo.attach(3);
	myServo.write(110);
}
 
void loop() 
{
	getIR();

	if(val == F) 
	{  
		forward();
	}

	if(val == B) 
	{  
		back();
	}

	if(val == R) 
	{  
		right();
	}

	if(val == L) 
	{  
		left();
	}

	if(val == S) 
	{  
		stop();
	}

	else 
	stop();	
}

void getIR()
{
	if (irrecv.decode(&results)) 
	{ 
		val = results.value;
		Serial.println(val);
		irrecv.resume();      // Receive the next value
		delay(150);
	}
}

void forward(){ 
  analogWrite(ENA,carSpeed);
  analogWrite(ENB,carSpeed);

  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  Serial.println("go forward!");
}
void back(){
  digitalWrite(ENA,carSpeed);
  digitalWrite(ENB,carSpeed);

  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  Serial.println("go back!");
}
void left(){
  analogWrite(ENA,carSpeed);
  analogWrite(ENB,carSpeed);

  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH); 
  Serial.println("go left!");
}
void right(){
  analogWrite(ENA,carSpeed);
  analogWrite(ENB,carSpeed);

  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  Serial.println("go right!");
}
void stop(){
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  Serial.println("STOP!");  
}

