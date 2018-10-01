#include <Servo.h> 

Servo throttle;
Servo steer;


const byte interruptPin = 2;
int ticks=0;


void setup() 
{ 
  Serial.begin(9600);
  throttle.attach(10);
  throttle.writeMicroseconds(1500);
  
  steer.attach(9);
  steer.writeMicroseconds(1500);// set servo to mid-point

  pinMode(interruptPin,INPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), tick, CHANGE);
} 

void loop() {
  throttle.writeMicroseconds(1300);
  Serial.println(ticks);
  for(int i=300;i<=2300;i+=10){
    steer.writeMicroseconds(i);
    delay(20);
  }
  Serial.println(ticks); 
  for(int i=2300;i>=300;i-=10){
    steer.writeMicroseconds(i);
    delay(20);
  }
  
  
  
} 


void tick(){
  ticks++;
}

