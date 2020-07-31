#include <Servo.h>

unsigned long msg;
unsigned long throttle;
unsigned long yaw;
unsigned long message;

char buffer[7];
char buf;
char *ptr;

int i;

Servo servo;
Servo esc;



void setup()
{
    Serial.begin(9600); 
    Serial1.begin(9600); 

    servo.attach(3);
    esc.attach(8);


}

void loop()
{

  if(Serial1.available()) // Don't read unless
                                                                      
  {
      while(i < 7) // One less than the size of the array
      {
          if(Serial1.available())
          {
             buf = Serial1.read(); 
             buffer[i] = buf; 
             i++; 
          }
        // buffer[i] = '\0'; // Null terminate the string
      }

      i = 0;
   //   Serial1.flush();
      Serial1.write('k');
  }


      //Serial.println(buffer);

       msg = strtoul(buffer, &ptr, 10);

      throttle = msg/1000;
      yaw = msg % 1000;

      Serial.print("Message: ");
      Serial.print(msg);        
      Serial.print("          Yaw: ");
      Serial.print(yaw);
      Serial.print("          Throttle: ");
      Serial.println(throttle);

     // delay(100);

      servo.write(yaw);
      esc.write(throttle);
}      
