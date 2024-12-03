#include <AFMotor.h>

AF_DCMotor motor1(1); // Connect motor 1 to port M1
AF_DCMotor motor2(2); // Connect motor 2 to port M2
AF_DCMotor motor3(3); // Connect motor 3 to port M3
AF_DCMotor motor4(4); // Connect motor 4 to port M4

const int forwardTime = 1000; // Time to move forward 4 meters (adjust as needed)
const int sideStepTime = 300; // Time to move sideways 0.5 meters (adjust as needed)
const int turnRightTime = 1200; // Time to turn 90 degrees right (adjust as needed)
const int turnLeftTime = 1150;  // Time to turn 90 degrees left (adjust as needed)
const int numPasses = 4; // Number of passes to cover the area

void setup() {
  Serial.begin(9600); // Initialize serial communication
  Serial.println("Robot initialized. Starting movement...");
}

void loop() {
  for (int i = 0; i < numPasses; i++) {
    // Move forward for 4 meters
    Serial.print("Pass ");
    Serial.print(i + 1);
    Serial.println(": Moving forward for 4 meters...");
    moveForward();
    delay(forwardTime);

    // Alternate turns after each pass
    if (i < numPasses - 1) { // Avoid turning after the last pass
      if (i % 2 == 0) {
        Serial.println("Turning right 90 degrees...");
        turnRight();
        delay(turnRightTime);

        Serial.println("Moving sideways for 0.5 meters...");
        moveForward();
        delay(sideStepTime);

        Serial.println("Turning right 90 degrees to align for the next pass...");
        turnRight();
        delay(turnRightTime);
      } else {
        Serial.println("Turning left 90 degrees...");
        turnLeft();
        delay(turnLeftTime);

        Serial.println("Moving sideways for 0. meters...");
        moveForward();
        delay(sideStepTime);

        Serial.println("Turning left 90 degrees to align for the next pass...");
        turnLeft();
        delay(turnLeftTime);
      }
    }
  }

  // Stop the robot
  Serial.println("Finished . Stopping the robot.");
  stop();
  while (true); // Stop the loop
}

void moveForward() {
  motor1.setSpeed(255); // Set speed to maximum (adjust as necessary)
  motor2.setSpeed(255);
  motor3.setSpeed(255);
  motor4.setSpeed(255);
  
  motor1.run(FORWARD); // Move forward
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void turnRight() {
  motor1.setSpeed(255);
  motor2.setSpeed(255);
  motor3.setSpeed(255);
  motor4.setSpeed(255);
  
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(FORWARD);
}

void turnLeft() {
  motor1.setSpeed(255);
  motor2.setSpeed(255);
  motor3.setSpeed(255);
  motor4.setSpeed(255);
  
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(BACKWARD);
}

void stop() {
  motor1.run(RELEASE); // Stop the motors
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}
