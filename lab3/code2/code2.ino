#include <Servo.h>
#include <SPI.h>
#include <SD.h>

const int chipSelect = 10;
long startTime = millis();                                // record time when processor powered up
int count = 0;


Servo throttle;
Servo steer;
Servo dropper;

int steer_write=1500;

int dist;
volatile byte state = LOW;

const byte interruptPin = 2;
const byte statePin = 3;
const byte chirpPin = 7;

const int STOP_DISTANCE = 2210;
const int CRUMB_DISTANCE = 160;

int ticks=0;


void setup()
{
  Serial.begin(9600);                                     // Open serial communications and wait for port to open:
  while (!Serial) {                                       // wait for serial port to connect. Needed for native USB port only
    ; 
  }

  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) {                            // see if the card is present and can be initialized:
    Serial.println("Card failed, or not present");
    return;                                               // if card failed, don't do anything else
  }
  Serial.println("card initialized.");

  File logFile = SD.open("testLog.txt", FILE_WRITE);      // open the file. note that only one file can be open at a time,
                                                          // so you have to close this one before opening another.
  
  logFile.println("loop_count,system_time,steer,dist");              // log column header info for csv file; do this only once during setup
  logFile.close();     
  throttle.attach(6);
  throttle.writeMicroseconds(1500);

  steer.attach(9);
  steer.writeMicroseconds(1500);// set servo to mid-point

  dropper.attach(5);
  dropper.write(300);

  pinMode(chirpPin,OUTPUT);
  digitalWrite(7,LOW);

  pinMode(interruptPin,INPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), tick, CHANGE);
  pinMode(statePin,INPUT);
  pinMode(statePin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(statePin), state_change, RISING);

}

void loop() {

  if (state && ticks < STOP_DISTANCE){
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
    Serial.println(dist);


    

    if (dist<30)
      steer_write=1680;
    else if (dist>50)
      steer_write=1320;
    //else
    //  steer.writeMicroseconds(1500);

    steer.writeMicroseconds(steer_write);

    if ((ticks%(CRUMB_DISTANCE))==0 && ticks>100)
    {
      for(int i=0;i<100;i++){
        dropper.write(50);
        delay(1);
      }
    }
    if ((ticks%(CRUMB_DISTANCE*2))==0&& ticks>100)
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
    /*
    String dataString = "";                                 // make a string for assembling the data to log
    dataString += String(count);                            // cast loop counter as String and add to data string to be logged
    float currentTime = (millis() - startTime) / 1000.0;    // find the amount of time transpired since processor powered (seconds)
    dataString += ",";                                      // add comma to seperate data fields in this row of data
    dataString += String(currentTime);                      // add elapsed system time to string of data to be recorded
    dataString += ",";                                      // add comma to seperate data fields in this row of data
    dataString += String(steer_write); 
    dataString += ",";                                      // add comma to seperate data fields in this row of data
    dataString += String(dist); 
    File logFile = SD.open("borncalderon.txt", FILE_WRITE);      // open file
    if (logFile) {                                          // if open and writeable
      logFile.println(dataString);                          // write row of data to csv file 
      logFile.close();                                      // close the file
      Serial.println(dataString);                           // print to the serial port too    
    }
    else {                                                  // if the file isn't open, pop up an error
        Serial.println("error opening datalog.txt");
    }
    count++; 
    */

    String dataString = "";                                 // make a string for assembling the data to log
    dataString += String(count);                            // cast loop counter as String and add to data string to be logged
    float currentTime = (millis() - startTime) / 1000.0;    // find the amount of time transpired since processor powered (seconds)
    dataString += ",";                                      // add comma to seperate data fields in this row of data
    dataString += String(currentTime);                      // add elapsed system time to string of data to be recorded
    dataString += ",";                                      // add comma to seperate data fields in this row of data
    dataString += String(steer_write); 
    dataString += ",";                                      // add comma to seperate data fields in this row of data
    dataString += String(dist); 
    File logFile = SD.open("testLog.txt", FILE_WRITE);      // open file
    if (logFile) {                                          // if open and writeable
      logFile.println(dataString);                          // write row of data to csv file 
      logFile.close();                                      // close the file
      Serial.println(dataString);                           // print to the serial port too    
    }
    else {                                                  // if the file isn't open, pop up an error
        Serial.println("error opening datalog.txt");
    }
    count++;                                                // increment loop counter
    //delay(1000);                                            // execute loop every 1 second
    
  }else{
    //Serial.println(state);
    throttle.writeMicroseconds(1500);
    delay(10);
    steer.writeMicroseconds(1500);
    delay(10);
  }

  if (ticks>=STOP_DISTANCE){
    throttle.writeMicroseconds(1500);
    delay(10);
    steer.writeMicroseconds(1500);
    delay(10);
    delay(200);
    digitalWrite(7,HIGH);
    delay(400);
    digitalWrite(7,LOW);
    throttle.writeMicroseconds(1500);
    delay(10);
    steer.writeMicroseconds(1500);
    delay(10);
    delay(200);
    digitalWrite(7,HIGH);
    delay(400);
    digitalWrite(7,LOW);
    delay(400);
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
