#include <EEPROM.h>

int id = 6000;
String value = "";
int state = 0; //current state of minion
int prev = 0;  //previous value of light intensity received
int trigger = A0;  //pin to read analog value from LDR
int trigger_led =2;
//Preferences preference;
float lat=0.0;
float lon=0.0;


void setup() {
  
  Serial.begin(9600);
  pinMode(trigger_led,OUTPUT);
  pinMode(trigger, INPUT_PULLUP);  
  prev = digitalRead(trigger);
//  preference.begin("Watch", true);
  load();
}

void loop() {  
  sig();  //called as frequenlty as possible to prevent delays between reading new values due to the processes  
 // wait(); 
  //check();   
  sendInfo();
  changeLocation();
}

void load(){
//  lat =  preference.getFloat("lat");
//  lon =  preference.getFloat("lon");
  EEPROM.get(0, lat);
  EEPROM.get(sizeof(float)+1, lon);
  if(lat>180)
    lat = 0;
  if(lon>180)
    lon=0;
//  Serial.println(lat, 6);
//  Serial.println(lon, 6);
  
}

void changeLocation(){
  
  if(Serial.available()>0){
    String input = "";    
    while(Serial.available()){
      input+=(char)Serial.read();
    }    
    if(input.equals("lat"))
      changeLat();  
    if(input.equals("lon"))
      changeLon(); 
  } 
}


void changeLat(){
  String input="";
  Serial.println("Please enter new latitude");
  while(!(Serial.available()));
//  while(Serial.available()){
//    input+=(char)Serial.read();
//  }
  input = Serial.readString();
  float coordinate=input.toFloat();
  if(coordinate<-180 || coordinate>180 || coordinate==0){
        Serial.println("");
        Serial.println("Please enter a non-zero number between -180 and +180");
  }
  else{
    lat = coordinate;
//    preference.putFloat("lat",lat);
    EEPROM.put(0, lat);
  }
    
}

void changeLon(){
  String input="";
  Serial.println("Please enter new longitude");
  while(!(Serial.available()));
//  while(Serial.available()){
//    input+=(char)Serial.read();
//  }
  input = Serial.readString();
  float coordinate=input.toFloat();
  if(coordinate<-180 || coordinate>180 || coordinate==0){
        Serial.println("");
        Serial.println("Please enter a non-zero number between -180 and +180");
  }
  else{
    lon = coordinate;
//    preference.putFloat("lon",lon);
    EEPROM.put(sizeof(float)+1, lon);
  }
    
}



int wait(){  
  
    sig();  //prevents delays between reading new values
    //wait();
  
  
}

/*checks if incoming message is meant for this minion*/
void check(){
  int pos = 0;
  char val;
  while (Serial.available()) {
    val = (char)Serial.read();
    if(val!=',')
    {
      value+=val;
    }
    else
      break;
  }   
  
  int vl = value.toInt();
  value = "";
  while (Serial.available()) {
    val = (char)Serial.read();
    if(val!='.')
    {
      value+=val;
    }
    else
      break;
  } 
  int v = value.toInt();
   
  if(v==1){
    state=0;
    digitalWrite(trigger_led,LOW);
   }
  value = "";
  sig();
  if(vl==id){    
   sendInfo(); 
   
  }
}


/*sends information to Master LoRa*/
void sendInfo(){  
  sig();
  String msg = "";
  char info[15];
  msg+=id;
  msg+=",";
//  float lat = 18.004366;
  msg+=String(lat,6);
  msg+=",";
//  float lon = -76.748279;  
  msg+=String(lon,6);
  msg+=",";
  msg+=state;
  Serial.println(msg); 
}

/*reads the voltage reading from the LDR to ascertain the light intensity*/
//void readIntensity(){
//  Serial.println(state);
//  int val = analogRead(trigger);
//  int dif = prev-val;
//  if(dif<0)
//    dif= dif*(-1);    
//  Serial.print("Value: ");
//  Serial.println(val); 
//  char msg[10];
//  sprintf(msg,"%-4d" ,val);  
//  u8x8.drawString(8, 2, msg);
//
//  if((dif>200) && (prev>val)){  
//    state=1;  
//    digitalWrite(trigger_led,HIGH);
//    Serial.println("Trigger");
//  }
//  prev = val;
//}

/*Called several times to prevent large delays between reading light intensity*/
void sig (){     
   //readIntensity();
   int val = !digitalRead(trigger);
   if(val==HIGH){
    state=1;
    digitalWrite(trigger_led,LOW);
   }
   else{
    state=0;
    digitalWrite(trigger_led,HIGH);
   }
}
