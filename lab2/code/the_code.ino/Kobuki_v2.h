/*
Kobuki.h - Library for interacting with the Kobuki
Created by Michael J. Conte, Pratheek Manjunath, July 7, 2017.
Released into the public domain.
*/
#ifndef Kobuki_v2_h
#define Kobuki_v2_h
#include "Arduino.h"

class Kobuki
{
  public:    
    void command(byte opcode, int sub_payload_1, int sub_payload_2 = '\0');
    bool refreshSensors();
    void processMessage();
    void printMessage();
    bool checkChecksum(byte *packet);
    int feedback(int sensor_id);
  private:
    byte rx_value[255];
    byte last_byte = 0x00;
};

#endif
