/*
  Line Follower Algorithm for Kobuki (Instructor version)
  Written by Pratheek Manjunath
  Updated by Dom Larkin (May 2018)
  This program uses the left and right clif sensors on the bottom of the robot to detect and follow a track(parallel) of reflective tape.
  

  Notes:
  i) The cliff sensor output values used in the 'while' loop may vary slightly depending on your robot platform and the environment. Therefore you must measure and verify the values it detects as the "tape" and the values it detects as the "floor".
  ii) These were my Cliff Sensor values when the robot was kept on the floor (grey tiles): Left=1600 ~ 1650; Center=1950 ~ 2000; Right=1490~1550
  iii) Cliff Sensor values when robot was kept on the tape (black glossy): Left=1350 ~ 1450; Center=1700 ~ 1800; Right=1370~1470. Tune 'rct' and 'lct' values.
  iv) The Arduino IDE will display a warning about a NULL field during compiling. It is harmless and you may disregard it.
  v) Useful tool - Binary/Decimal/Hexadecimal converter: https://www.mathsisfun.com/binary-decimal-hexadecimal-converter.html

  Protocol Commands:
  kobuki.command(1, 100, 0x0040); // Turn left
  kobuki.command(1, 100, 0xffd8); // Turn right
  kobuki.command(1, 100, 0);      // Straight
  kobuki.command(1, 0, 0);        // Stop
  kobuki.command(4, 5);           // Play a sound
  kobuki.feedback(14);            // B0 Button (int)
  kobuki.feedback(34);            // Right IR Cliff Sensor (int)
  kobuki.feedback(36);            // Center IR Cliff Sensor (int)
  kobuki.feedback(38);            // Left IR Cliff Sensor (int)
*/

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
  
  //Serial.println("In loop");
  int mag_check = analogRead(0);
  //Serial.println(mag_check);
  
  if(mag_check>950){
    Serial.println("Gate Opened");
    Serial.println(mag_check);
    the_servo.write(200);
    delay(100);
  }else{
    //the_servo.write(93);
    //delay(100);
  }
  
  //kobuki.refreshSensors();
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
    if (kobuki.feedback(34) > rct && kobuki.feedback(38) > lct)  //  Right & Left dont' see the line
    {
      kobuki.command(1, 100, 0);   // Straight
    }
  }

  //delay(10);
  
  
}


void open_the_gates(){
  
  delay(1000);
  
}

