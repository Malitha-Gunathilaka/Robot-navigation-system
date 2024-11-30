#include <AFMotor.h>
#include <Wire.h>
#include <QMC5883LCompass.h>

// Motor initialization for layout:
//    M1    M2  (Front)
//     ┌────┐
//     │    │
//     │    │
//     └────┘
//    M4    M3  (Back)

AF_DCMotor motor1(1); // Front Left
AF_DCMotor motor2(2); // Front Right
AF_DCMotor motor3(3); // Back Right
AF_DCMotor motor4(4); // Back Left

// Compass setup
QMC5883LCompass compass;
float initialHeading;
const int motorSpeed = 220;  // Reduced for better control
const float HEADING_TOLERANCE = 3.0;

// Badminton court parameters
const float COURT_LENGTH = 13.4;
const float COURT_WIDTH = 6.1;
const int MOVE_TIME = 1000;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  Serial.println("Initializing compass...");
  compass.init();
  compass.setCalibration(-1309, 1729, -1813, 1307, -1563, 1604);
  compass.setSmoothing(10, true);

  Serial.println("Calibrating compass...");
  calibrateCompass();

  initialHeading = getHeading();
  Serial.print("Initial heading: ");
  Serial.println(initialHeading);
  delay(1000);
}

void loop() {
  for (int pass = 0; pass < 4; pass++) {
    Serial.print("Pass ");
    Serial.println(pass + 1);

    moveForwardWithCompass(MOVE_TIME * 2);
    delay(500);

    if (pass < 3) {
      turnRight90();
      delay(500);
      moveForwardWithCompass(500);
      delay(500);
      turnRight90();
    }
  }

  stop();
  Serial.println("Navigation complete");
  while (1);
}

void calibrateCompass() {
  Serial.println("Starting compass calibration...");
  const int calibrationSpeed = 100;
  const int totalRotationTime = 8000;

  // Gradual speed increase
  for (int speed = 50; speed <= calibrationSpeed; speed += 10) {
    setAllMotorSpeeds(speed);
    delay(100);
  }

  unsigned long startTime = millis();
  while (millis() - startTime < totalRotationTime) {
    motor1.run(FORWARD);
    motor2.run(BACKWARD);
    motor3.run(BACKWARD);
    motor4.run(FORWARD);

    compass.read();
    Serial.print("Heading: ");
    Serial.println(compass.getAzimuth());
    delay(100);
  }

  // Gradual stop
  for (int speed = calibrationSpeed; speed >= 0; speed -= 10) {
    setAllMotorSpeeds(speed);
    delay(100);
  }

  stop();
  Serial.println("Compass calibration complete");
}

float getHeading() {
  compass.read();
  float azimuth = compass.getAzimuth();
  return azimuth;
}

float calculateHeadingError(float target, float current) {
  float error = target - current;
  if (error > 180) error -= 360;
  if (error < -180) error += 360;
  return error;
}

void moveForwardWithCompass(int duration) {
  unsigned long startTime = millis();

  while (millis() - startTime < duration) {
    float currentHeading = getHeading();
    float error = calculateHeadingError(initialHeading, currentHeading);

    int leftSpeed = motorSpeed;
    int rightSpeed = motorSpeed;

    if (abs(error) > HEADING_TOLERANCE) {
      if (error > 0) {
        rightSpeed -= (abs(error) * 2);
      } else {
        leftSpeed -= (abs(error) * 2);
      }
    }

    // Constrain speeds
    leftSpeed = constrain(leftSpeed, 0, 255);
    rightSpeed = constrain(rightSpeed, 0, 255);

    // Set speeds for motors
    motor1.setSpeed(leftSpeed);   // Front Left
    motor2.setSpeed(rightSpeed);  // Front Right
    motor3.setSpeed(rightSpeed);  // Back Right
    motor4.setSpeed(leftSpeed);   // Back Left

    // Move forward
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    motor3.run(FORWARD);
    motor4.run(FORWARD);

    delay(50);
  }
  stop();
}

void turnRight90() {
  // Update the initial heading
  initialHeading += 90.0;
  if (initialHeading >= 360.0) initialHeading -= 360.0;

  float error;
  do {
    float currentHeading = getHeading();
    error = calculateHeadingError(initialHeading, currentHeading);

    // Set motor speeds
    setAllMotorSpeeds(motorSpeed);

    // Turn right
    motor1.run(FORWARD);   // Front Left
    motor2.run(BACKWARD);  // Front Right
    motor3.run(BACKWARD);  // Back Right
    motor4.run(FORWARD);   // Back Left

    delay(50);
  } while (abs(error) > HEADING_TOLERANCE);

  stop();
}

void setAllMotorSpeeds(int speed) {
  motor1.setSpeed(speed);
  motor2.setSpeed(speed);
  motor3.setSpeed(speed);
  motor4.setSpeed(speed);
}

void stop() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
  delay(100);
}