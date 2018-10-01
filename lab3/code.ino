/*

Example taken from: https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/

*/


const byte interruptPin = 7;
int count=0;

void setup() {
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), tick, CHANGE);
}

void loop() {
	
}

void tick() {
  count++;
}