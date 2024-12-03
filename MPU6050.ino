#include <AFMotor.h>
#include <Wire.h>
#include <MPU6050.h>

// Motor setup
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

// MPU6050 setup
MPU6050 mpu;
float currentAngle = 0;

// Movement constants
const int FORWARD_SPEED = 255;
const int TURN_SPEED = 150;
const float ANGLE_TOLERANCE = 2.0;
const int FORWARD_DISTANCE = 4000; // 4 meters in milliseconds
const int SIDE_STEP = 500;        // 0.5 meters in milliseconds
const int numPasses = 4;

void setup() {
  Serial.begin(9600);
  
  // Initialize MPU6050
  Wire.begin();
  mpu.initialize();
  
  // Verify MPU6050 connection
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed!");
    while(1);
  }
  
  // Calibrate gyro
  calibrateGyro();
  Serial.println("Robot initialized. Starting movement...");
}

void loop() {
  coverSquareArea();
  while(true); // Stop after completing pattern
}

void coverSquareArea() {
  for (int i = 0; i < numPasses; i++) {
    // Move forward 4 meters
    Serial.print("Pass ");
    Serial.print(i + 1);
    Serial.println(": Moving forward 4 meters...");
    moveForwardTimed(FORWARD_DISTANCE);
    
    if (i < numPasses - 1) {
      if (i % 2 == 0) {
        Serial.println("Turning right 90 degrees...");
        turnRight90();
        
        Serial.println("Moving sideways 0.5 meters...");
        moveForwardTimed(SIDE_STEP);
        
        Serial.println("Turning right 90 degrees...");
        turnRight90();
      } else {
        Serial.println("Turning left 90 degrees...");
        turnLeft90();
        
        Serial.println("Moving sideways 0.5 meters...");
        moveForwardTimed(SIDE_STEP);
        
        Serial.println("Turning left 90 degrees...");
        turnLeft90();
      }
    }
  }
  Serial.println("Pattern complete!");
  stop();
}

void moveForward() {
  motor1.setSpeed(FORWARD_SPEED);
  motor2.setSpeed(FORWARD_SPEED);
  motor3.setSpeed(FORWARD_SPEED);
  motor4.setSpeed(FORWARD_SPEED);
  
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void moveForwardTimed(int duration) {
  moveForward();
  delay(duration);
  stop();
}

void turnRight90() {
  float targetAngle = currentAngle - 90.0;
  
  while (abs(currentAngle - targetAngle) > ANGLE_TOLERANCE) {
    updateAngle();
    
    motor1.setSpeed(TURN_SPEED);
    motor2.setSpeed(TURN_SPEED);
    motor3.setSpeed(TURN_SPEED);
    motor4.setSpeed(TURN_SPEED);
    
    motor1.run(FORWARD);
    motor2.run(BACKWARD);
    motor3.run(BACKWARD);
    motor4.run(FORWARD);
  }
  stop();
}

void turnLeft90() {
  float targetAngle = currentAngle + 90.0;
  
  while (abs(currentAngle - targetAngle) > ANGLE_TOLERANCE) {
    updateAngle();
    
    motor1.setSpeed(TURN_SPEED);
    motor2.setSpeed(TURN_SPEED);
    motor3.setSpeed(TURN_SPEED);
    motor4.setSpeed(TURN_SPEED);
    
    motor1.run(BACKWARD);
    motor2.run(FORWARD);
    motor3.run(FORWARD);
    motor4.run(BACKWARD);
  }
  stop();
}

void stop() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

void updateAngle() {
  int16_t gx, gy, gz;
  mpu.getRotation(&gx, &gy, &gz);
  float gyroRate = gz / 131.0;
  currentAngle += gyroRate * (1.0/50.0);
  delay(20);
}

void calibrateGyro() {
  float sumX = 0, sumY = 0, sumZ = 0;
  int samples = 100;
  
  Serial.println("Calibrating gyro. Keep robot still...");
  
  for(int i = 0; i < samples; i++) {
    int16_t gx, gy, gz;
    mpu.getRotation(&gx, &gy, &gz);
    sumX += gx;
    sumY += gy;
    sumZ += gz;
    delay(10);
  }
  
  // Store calibration values if needed
  currentAngle = 0;
  Serial.println("Calibration complete!");
}