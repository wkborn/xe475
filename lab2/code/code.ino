/*
LAB 2
XE472
CDT Born '19 H-2 and CDT Belardo-Flowers '19 G-2
DUE At Start of Lab 3 / 1 OCT 2018

TODO:
1) set up robot hardware
2) define sensors
3) design state machine
4) interface w/ servos
5) ???
6) Profit


*/

#include<stdint.h>
#include "Kobuki_v2.h"

Kobuki kobuki;

uint16_t ir_left;
uint16_t ir_right;

void setup() {
  Serial.begin(9600);
  Serial1.begin(115200);  // To communicate with the robot
  

  Serial.print("once\n");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("forever\n");

  kobuki.refreshSensors();
  if (kobuki.refreshSensors()){
    kobuki.command(1, 100, 0);
  
    sensors();
    delay(100);
    state_machine(set_flags());
  }
  delay(1000);
}

void sensors(){
  Serial.print("collecting sensors\n");
  Serial.print("sensor data written\n");
}

uint8_t set_flags(){
  Serial.print("setting flags\n");

  //TODO set flag code

  uint8_t ir_right_flag= 0b00000001;
  //uint8_t ir_left_flag= 0b00000010;
  uint8_t bot_detect_flag= 0b00000100;

  //uint8_t ir_right_flag= 0b00000000;
  uint8_t ir_left_flag= 0b00000000;
  //uint8_t bot_detect_flag= 0b00000000;

  uint8_t state = ir_right_flag | ir_left_flag | bot_detect_flag;
  
  Serial.print("passing flags\n");
  delay(50);
  return state;
}

void state_machine(int state)
{
  Serial.print("received flag ");Serial.print(state);Serial.print("\n");
  switch(state){
    case 0:
      Serial.print("State 0\n");
      //TODO
    break;
    case 1:
      Serial.print("State 1\n");
      //TODO
    break;
    case 2:
      Serial.print("State 2\n");
      //TODO
    break;
    case 3:
      Serial.print("State 3\n");
      //TODO
    break;
    case 4:
      Serial.print("State 4\n");
      //TODO
    break;
    case 5:
      Serial.print("State 5\n");
      //TODO
    break;
    case 6:
      Serial.print("State 6\n");
      //TODO
    break;
    case 7:
      Serial.print("State 7\n");
      //TODO
    break;
    delay(1);
  }
}

