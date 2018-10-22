#include <Wire.h> // Reference the I2C Library
#include "HMC5883L.h" // Reference the HMC5883L Compass Library
HMC5883L compass; // Store our compass as a variable.

unsigned long lastTime = millis(); //Initialize variable that represents the time after a reading successfully completes
unsigned long currentTime = millis(); //Initialize variable that represents the current time
unsigned long loopRate = 66; //Default bandwidth for the HMC5883L. If currentTime-lastTime>loopRate, sample magnetometer
float headingLast;

int error = 0; // Record any errors that may occur in the compass.
void setup() {
 //Same setup as read_compass_example_v2 code made by LTC Lowrance
 Serial.begin(9600); // Initialize the serial port.

 Serial.println("Starting the I2C interface.");
 Wire.begin(); // Start the I2C interface.

 Serial.println("Constructing new HMC5883L");
 compass = HMC5883L(); // Construct a new HMC5883 compass.
  
 Serial.println("Setting scale to +/- 1.3 Ga");
 error = compass.SetScale(1.3); // Set the scale of the compass.
 if(error != 0) // If there is an error, print it out.
  Serial.println(compass.GetErrorText(error));
 
 Serial.println("Setting measurement mode to continous.");
 error = compass.SetMeasurementMode(Measurement_Continuous); // Set the measurement mode to Continuous
 if(error != 0) // If there is an error, print it out.
  Serial.println(compass.GetErrorText(error));
}

void loop() {
  currentTime=millis(); //poll for current time
  if (currentTime-lastTime>loopRate){ //run the sampling code in this conditional
    // Retrive the raw values from the compass (not scaled).
    //MagnetometerRaw raw = compass.ReadRawAxis();
    // Retrived the scaled values from the compass (scaled to the configured scale).
    MagnetometerScaled scaled = compass.ReadScaledAxis();
 
    // Values are accessed like so:
    //int MilliGauss_OnThe_XAxis = scaled.XAxis;// (or YAxis, or ZAxis)

    // Calculate heading when the magnetometer is level, then correct for signs of axis.
    float heading = atan2(scaled.YAxis, scaled.XAxis);
  
    // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
 // Mine is: 13 deg 4' W, which is 13.07 Degrees, or (which we need) 0.228056 radians, I will use 0.2281. Found at http://www.magnetic-declination.com/
 float declinationAngle = 0.2281;
 heading += declinationAngle;
 
 // Correct for when signs are reversed.
 if(heading < 0)
  heading += 2*PI;
  
 // Check for wrap due to addition of declination.
 if(heading > 2*PI)
  heading -= 2*PI;
 
 // Convert radians to degrees for readability.
 float headingDegrees = heading * 180/M_PI; 

 // Output the data via the serial port.
 Output(heading, headingDegrees);
 //Serial.print("  \tHeading Last:\t");
 //Serial.println(headingLast);
 turnDirection(heading, headingLast);
  headingLast = heading;
  lastTime = millis(); // Take last time
  }

}

// Output the data down the serial port.
void Output(float heading, float headingDegrees)
{
 
 //Serial.print("  \tHeading:\t");
 //Serial.print(heading);
 //Serial.print(" Radians  \t");
 //Serial.print(headingDegrees);
 //Serial.println(" Degrees  \t");
}

void turnDirection(float heading, float headingLast) {
  if (heading-headingLast>0.05) { //Check to see the difference between headings during each poll. 
    //If the difference is larger than a random noise difference (about 0.05 radians), turn the motor
    Serial.println("Turn Motor Counter Clockwise");
  }
  else if (heading-headingLast<-0.05) {
    Serial.println("Turn Motor Clockwise");
  }
  else {
    Serial.println("No Action Necessary"); //No turn is necessary when the difference between polled values is less than 
    //that of random noise (about 0.05 radians)
  }
}

float getHeading() {              //function used for reading compass; returns orientation in degrees
  // Get the magnetometer reading from the sensor and calculate the heading
  MagnetometerScaled sensor = compass.ReadScaledAxis();     //scaled values from compass.
  float heading = atan2(sensor.YAxis, sensor.XAxis);        // Key formula to obtain compass heading from magnetic field
  return rad2deg(heading);                                  //Convert from radians to degrees
}

float rad2deg(float heading) {
  // Correction when signs are reversed or for wrap around (return 0-360 degrees)
  if (heading < 0) heading += 2 * PI;
  if (heading > 2 * PI) heading -= 2 * PI;
  return heading * 180 / PI;
}
