#include <Servo.h> 

Servo throttle;
Servo steer;
Servo dropper;

int dist;
volatile byte state = LOW;

const byte interruptPin = 2;
const byte statePin = 3;
int ticks=0;


void setup() 
{ 
  Serial.begin(9600);
  throttle.attach(10);
  throttle.writeMicroseconds(1500);
  
  steer.attach(9);
  steer.writeMicroseconds(1500);// set servo to mid-point

  dropper.attach(5);
  dropper.write(300);

  pinMode(interruptPin,INPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), tick, CHANGE);
  pinMode(statePin,INPUT);
  pinMode(statePin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(statePin), state_change, RISING);
  
} 

void loop() {
  
  if (state){
    if(ticks==0){
      throttle.writeMicroseconds(1370);
      delay(10);
      throttle.writeMicroseconds(1370);
      delay(10);
      throttle.writeMicroseconds(1370);
      delay(10);
      throttle.writeMicroseconds(1370);
      delay(10);
    }
    dist = analogRead(0);
    Serial.println(ticks);
    
    if (dist<20)
      steer.writeMicroseconds(1680);
    else if (dist>50)
      steer.writeMicroseconds(1320);
    else
      steer.writeMicroseconds(1500);
  
    if ((ticks%300)==0 && ticks>100)
    {
      for(int i=0;i<100;i++){
        dropper.write(50);
        delay(1);
      }
    }
    if ((ticks%600)==0&& ticks>100)
    {
      for(int i=0;i<100;i++){
        dropper.write(270);
        delay(1);
      }
    }
  
    if ((ticks%50)==0&& ticks>100)
    {
      throttle.writeMicroseconds(1370);
    }
  }else{
    Serial.println("low");
    throttle.writeMicroseconds(1500);
    delay(10);
    steer.writeMicroseconds(1500);
    delay(10);
  }
} 


void tick(){
  ticks++;
}

void state_change(){
  if(state==LOW)
    throttle.writeMicroseconds(1370);
  state = !state;
}
