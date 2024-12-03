#include <AFMotor.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

// Create compass instance with unique ID
Adafruit_HMC5883_Unified compass = Adafruit_HMC5883_Unified(12345);
const float TURN_TOLERANCE = 2.0;

void setup() {
  Serial.begin(9600);
  
  // Initialize HMC5883L
  if(!compass.begin()) {
    Serial.println("HMC5883L not detected. Check wiring!");
    while(1);
  }
}

float getHeading() {
  sensors_event_t event;
  compass.getEvent(&event);
  
  float heading = atan2(event.magnetic.y, event.magnetic.x);
  
  // Correct for heading < 0deg and > 360deg
  if(heading < 0)
    heading += 2*PI;
  if(heading > 2*PI)
    heading -= 2*PI;
    
  // Convert radians to degrees
  float headingDegrees = heading * 180/M_PI;
  
  return headingDegrees;
}

// Rest of the code remains the same
void turnToHeading(float targetHeading) {
  float currentHeading = getHeading();
  float error;
  
  while(true) {
    currentHeading = getHeading();
    error = targetHeading - currentHeading;
    
    if(error > 180) error -= 360;
    if(error < -180) error += 360;
    
    if(abs(error) <= TURN_TOLERANCE) {
      stopMotors();
      break;
    }
    
    if(error > 0) {
      // Turn right
      motor1.setSpeed(150);
      motor2.setSpeed(150);
      motor3.setSpeed(150);
      motor4.setSpeed(150);
      motor1.run(FORWARD);
      motor2.run(FORWARD);
      motor3.run(BACKWARD);
      motor4.run(BACKWARD);
    } else {
      // Turn left
      motor1.setSpeed(150);
      motor2.setSpeed(150);
      motor3.setSpeed(150);
      motor4.setSpeed(150);
      motor1.run(BACKWARD);
      motor2.run(BACKWARD);
      motor3.run(FORWARD);
      motor4.run(FORWARD);
    }
    delay(10);
  }
}

void turnRight() {
  float currentHeading = getHeading();
  float targetHeading = currentHeading + 90;
  if(targetHeading >= 360) targetHeading -= 360;
  turnToHeading(targetHeading);
}

void turnLeft() {
  float currentHeading = getHeading();
  float targetHeading = currentHeading - 90;
  if(targetHeading < 0) targetHeading += 360;
  turnToHeading(targetHeading);
}

void stopMotors() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}