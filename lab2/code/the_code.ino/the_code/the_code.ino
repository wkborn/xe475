//CDT Born and CDT Belardo-Flowers

#include "Kobuki_v2.h"
#include <Servo.h>

Servo the_servo;  // create servo object to control a servo
int pos = 0; 

Kobuki kobuki;  // declares a variable called 'kobuki' of Class 'Kobuki'.

int rct = 1580; // ADC thresholds for decision making - Right cliff sensor with tape
int lct = 1580; // left cliff sensor with tape

void setup()
{
  Serial1.begin(115200);  // To communicate with the robot
  Serial.begin(9600);     // To communicate with the user via Serial Monitor

  pinMode(2, INPUT);
  
  the_servo.attach(6);
  the_servo.write(0);
  delay(500);
  the_servo.write(92);

  Serial.println("Robot Ready!");

  int is_ready=0;
  pinMode(2, INPUT);

  while(is_ready==0){
    Serial.println("waiting");
    is_ready = digitalRead(2);
  }

  kobuki.command(1, 100, 0);   // Straight
  
}

void loop()
{
 
  int mag_check = analogRead(0);
  
  if(mag_check>950){
    Serial.println("Gate Opened");
    Serial.println(mag_check);
    the_servo.write(200);
    delay(100);
  }
  
  if (kobuki.refreshSensors())  // Proceed only if there are no errors in packet transmission. If a byte-stream is incomplete/misaligned, refreshSensors() returns a boolean 'ture' indicating an error.
  {
    Serial.println("No errors!");
    
    if (kobuki.feedback(34) > rct && kobuki.feedback(38) < lct) // Left cliff detects line
    {
      kobuki.command(1, 100, 0x0040);  // Turn left
      delay(100);
    }
    else if (kobuki.feedback(34) < rct && kobuki.feedback(38) > lct)  // Right cliff detects line
    {
      kobuki.command(1, 100, 0xffd8);  // Turn right
      delay(100);
    }
    else if (kobuki.feedback(34) > rct && kobuki.feedback(38) > lct)  //  Right & Left dont' see the line
    {
      kobuki.command(1, 100, 0);   // Straight
    }
    else{
      while(1){
        kobuki.command(1, 0, 0);
      }
    }
  }
  
}
