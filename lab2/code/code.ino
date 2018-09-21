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

void setup() {
  Serial.begin(9600);

  Serial.print("once\n");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("forever\n");

  delay(500);

  sensors();
  state_machine(set_flags());
}

int set_flags(){
  
}

void state_machine(int state)
{
  switch(state){
    case 0:
      //TODO
    break;
    case 1:
      //TODO
    break;
    case 2:
      //TODO
    break;
    case 3:
      //TODO
    break;
    case 4:
      //TODO
    break;
    case 5:
      //TODO
    break;
    case 6:
      //TODO
    break;
    case 7:
      //TODO
    break;
    
  }
}

