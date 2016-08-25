/*
 *AUTO HEADLIGHT 101 V1.0a 
*/
#include <EEPROM.h>                               //Store data when vehicle is turn off.
int address = 512;                                //Setting the location for threshold values to be stored.
int thresholdvalue = 0;                           //Threshold value for light activation.
int lowbound= 0, highbound= 0;                    //Limit for calibration
int outputpin = 13;                               //Set the ouput pin to control light.
int statuslight = 12, statuslight2 = 11;          //Set status light pin
int enter = 3;                                    //Set input pin to change the threshold level.
int analogpin = 0;                                //Set light sensor input pin.
int averageValue[5];
int state = 0;                                    //Variable for button debouncing.
long prevtime = 0;
bool trigger = 0;
long timer;
int counter =0;
bool alarmstate = 1;
long serialupdate =0;
long timerstatus;
//RPM/SPEED READOUT
int rpm;
const int sensorPin = 2;
volatile unsigned long lastPulseTime; 
volatile unsigned long interval = 0; 

//toggle enabletone out
long toneprevtime;
bool tonestatus =0;
bool enabletone = 1;


void setup() {
  // put your setup code here, to run once:
  thresholdvalue = EEPROM.read(address);
  pinMode(outputpin, OUTPUT);
  pinMode(statuslight, OUTPUT);
  pinMode(statuslight2, OUTPUT);
  pinMode(5, OUTPUT);                     //tone
  pinMode(4, INPUT_PULLUP);
  pinMode(enter, INPUT_PULLUP);
  attachInterrupt(0, sensorIsr, RISING); //enable rpm readout
  lastPulseTime = micros(); 
  Serial.begin(9600);
  Serial.print("Stored threshold value:\t\t");
  Serial.println(thresholdvalue);

}

void loop() {
  //CHECK IF SENSOR VALUE EXCEED THRESHOLD VALUE
  if(millis()-serialupdate > 1000){
    Serial.print("Current speed in RPM:\t\t");
    Serial.println(rpm);
    serialupdate = millis();
  }
  rpm = long(60e6)/interval;
  if((analogRead(analogpin)/4 < thresholdvalue) && rpm > 6000){
    digitalWrite(statuslight,1);
    trigger = 1;    
  }
  else{
    digitalWrite(statuslight,0);
    digitalWrite(outputpin,0);
    timerstatus = millis();
    trigger = 0;       
  }
  if((millis()-timerstatus> 2000)&&(trigger == 1)){
    fastflash();
  }
  if(digitalRead(4) == 1){
    tonestatus = 0;
    toneprevtime= millis();
  }
  if(!tonestatus && digitalRead(4) == 0 && millis()-toneprevtime >25){
    tonestatus = 1;
    enabletone = !enabletone;
  }
  if(enabletone == 1){
    tone(5, 34);
  }
  else if(enabletone == 0){
    tone(5, 166);
  }
   
  //ENTER CALIBRATION IF BUTTON PRESSED FOR 1.5 SECONDS OR MORE
  if((digitalRead(3) == 0)&&millis()-prevtime > 1500){
    state = 1;
    prevtime = millis();
  }
  else if (digitalRead(3) == 1){
    prevtime = millis();
  }


  
  //ENTER CALIBRATION MENU
  if(state == 1){
    state = 0;
    thresholdvalue = 0;
    digitalWrite(statuslight,1);
    for(int x = 0; x<5; x++){
      averageValue[x] = analogRead(analogpin);
      thresholdvalue = thresholdvalue + averageValue[x];
      delay(250);
    }
    lowbound = thresholdvalue / 20;
    digitalWrite(statuslight2,1);
    delay(3000);
    digitalWrite(statuslight,0);
    thresholdvalue = 0;
    for(int x = 0; x<5; x++){
      averageValue[x] = analogRead(analogpin);
      thresholdvalue = thresholdvalue + averageValue[x];
      delay(250);
    }
    highbound = thresholdvalue / 20;  
    thresholdvalue = (highbound + lowbound)/2;    
    Serial.print("Average Threshold Value:\t");
    Serial.println(thresholdvalue);
    EEPROM.write(address,thresholdvalue);
    digitalWrite(statuslight,0);
    digitalWrite(statuslight2,0);
  }
  //FINISH CALIBRATION MENU
}

void fastflash(){
  if((counter-9)%10==0){     
    if(millis() - timer >= 300){
      digitalWrite(13,alarmstate);
      alarmstate=!alarmstate;
      counter++;
      timer = millis();
    }
  }
  else if(counter>=1000){
    digitalWrite(13,1);
  }
  else{
    if(millis() - timer >= 75){
      digitalWrite(13,alarmstate);
      alarmstate=!alarmstate;
      counter++;
      timer = millis();
    }
  }
}

void sensorIsr(){
  unsigned long now = micros(); 
  interval = now - lastPulseTime; 
  lastPulseTime = now; 
} 
