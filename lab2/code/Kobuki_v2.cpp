/*
Kobuki_v2

This is a library to provide a serail communication interface with the Kobuki robot.
This code assumes the Kobuki is on Serial1. TODO allow this to be specified to the interface.

The Circuit:
	+5v and gound from robot to arduino
	robot TX to arduino RX
	robot RX to arduino TX

Created September 12, 2017
By Michael J. Conte and Pratheek Manjunath
Modified October 8, 2017
By Dominic Larkin
*/
#include "Arduino.h"													// gives the source file access to all arduino commands such as Serial1.print
#include "Kobuki_v2.h"

void Kobuki::command(byte opcode, int sub_payload_1, int sub_payload_2)	// formats the bytestream that is being sent to the kobuki  
{
	byte data_length = 0x00;					
	byte payload[6] = {opcode, 0, 0, 0, 0, 0};							// payload is an array of the data you want to send to the kobuki
	
	if (opcode == 1)
	{
		data_length = 0x04;
		payload[1] = data_length;
		payload[2] = lowByte(sub_payload_1);							// the kobuki reads each element of the stream as a byte, therefore any value that is larger than one byte must be seperated into two one-byte elements					
		payload[3] = highByte(sub_payload_1);
		payload[4] = lowByte(sub_payload_2);
		payload[5] = highByte(sub_payload_2);
	}
	else if (opcode == 3)
	{
		data_length = 0x03;
		payload[1] = data_length;
		payload[2] = highByte(sub_payload_1);
		payload[3] = lowByte(sub_payload_1);
		payload[4] = lowByte(sub_payload_2);
	}
	else if (opcode == 4)
	{
		data_length = 0x01;
		payload[1] = data_length;
		payload[2] = lowByte(sub_payload_1);
	}
	else if (opcode == 12)
	{
		data_length = 0x02;
		payload[1] = data_length;
		payload[2] = highByte(sub_payload_1);
		payload[3] = lowByte(sub_payload_1);
	}
	else
	{
		payload[1] = data_length;
	}
	
	int i;
	byte header1 = 0xAA, header2 = 0x55, len = 2 + data_length, len_of_packet = len + 3;	// header1/header2 let the kobuki know that this is the beginning of the bytestream
	byte checksum = 0x00, packet[len_of_packet] = {header1, header2, len};					// checksum is the result of xoring all elements of the bytestream, that lets the kobuki know id the data has been corrupted. packet is the final bytestream
		
	checksum ^= len;
	checksum ^= opcode;
	checksum ^= data_length;
	for (i = 0; i < len; i++)
	{
		checksum ^= payload[i];
	}
	for (i = 0; i < len; i++)																	
	{
		packet[i + 3] = payload[i];
	}
	packet[len + 3] = checksum;
	Serial1.write(packet, len_of_packet);
}

bool Kobuki::refreshSensors()
{
    byte ch;
    if (Serial1.available() > 0 ) {				// Only refresh sensors if there are bytes ready to be read
      ch = Serial1.read();						// Read the bytes while looking for a header
      if (ch == 0x55 && last_byte == 0xAA) {    // If the 2-byte header is found begin processing a message
        processMessage(); 						// Update the state variable rx_values with message bytes
        return(checkChecksum(rx_value));		// If the message has a valid checksum return true else false
      }
      last_byte=ch;								// Update last_byte, used to find 2-byte header
    }
	return false;
}

void Kobuki::processMessage(){
  int indx = 0;								// Used as the index into the byte array
  while (Serial1.available() == 0 ) {}		// Blocking wait for a byte to arrive in the buffer
  //rx_value[indx++]=0x55;
  //rx_value[indx++]=0xAA;
  int msg_length=Serial1.read();			// The next byte after the header is the length of the message
  rx_value[indx++] = msg_length;  			// Store the message length as the first byte in the byte array	
  while(indx <= msg_length+1){				// Keep reading bytes until we have read message length of bytes
    while(Serial1.available()){				// While there is a byte in the buffer 
      rx_value[indx++] = Serial1.read();    // Read the contents of the buffer    
    }
  }  
}

// Print a byte in array with the bytes seperated by a comma
void Kobuki::printMessage(){				
  for(int i=0;i<=rx_value[0]+1;i++){
    Serial.print(rx_value[i],HEX);
    Serial.print(",");
  }
  Serial.println();
}

// Compute the checksum of a message and compare it to the expected checksum
bool Kobuki::checkChecksum(byte *packet){
  int packet_length = packet[0];  
  byte check_sum = 0; 							// Stores the computed checksum
  for (int i=0;i<=packet_length;i++){
	  {check_sum ^= packet[i];}					// Add the current byte to the checksum
  }
  if (check_sum == packet[packet_length+1]){	// The expected checksum is stored at message length + 1
	  return true;								// If the calculated checksum is valid		
	  }
  else {
	  return false;								// If the calculated checksum is invalid
	  }
}

int Kobuki::feedback(int sensor_id)	// returns the output of a specific sensor depending on the ID the user gives.
{
	if ((sensor_id == 8) || (sensor_id == 10) || (sensor_id == 25) || (sensor_id == 27) || (sensor_id == 34) || (sensor_id == 36) || (sensor_id == 38) )	//some sensors output data longer than one byte, if the user wants one of these values then this portion concatenates the bytes together into a readable form
	{
		int value = (rx_value[sensor_id + 1] << 8) | rx_value[sensor_id];  // Combines bytes for messages with data in 2 bytes
		return value;
	}
	else
	{
		return (int)rx_value[sensor_id];																																
	}	
}
