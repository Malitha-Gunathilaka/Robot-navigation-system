#include <AFMotor.h>

// Motor setup
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

// Badminton court dimensions in meters
const float COURT_LENGTH = 13.4;  // Length
const float COURT_WIDTH = 6.1;    // Width

// Movement parameters
const int forwardTime = 800;      // Time to move forward ~2 meters
const int sideStepTime = 250;     // Time to move sideways ~0.5 meters
const int turnTime = 1000;        // Time for 90-degree turn
const int numPasses = 12;         // Passes to cover court width
const int motorSpeed = 200;       // Reduced speed for better control

void setup() {
  Serial.begin(9600);
  Serial.println("Badminton Court Robot initialized...");
}

void loop() {
  float currentX = 0;
  float currentY = 0;
  
  for (int i = 0; i < numPasses; i++) {
    // Move forward within court length
    Serial.print("Pass ");
    Serial.println(i + 1);
    moveForward();
    delay(forwardTime);
    currentY += 2.0;  // Approximate distance covered

    // Check if we're still within court
    if (currentY >= COURT_LENGTH) {
      stop();
      break;
    }

    if (i < numPasses - 1) {
      if (i % 2 == 0) {
        turnRight();
        delay(turnTime);
        moveForward();
        delay(sideStepTime);
        currentX += 0.5;  // Side step distance
        turnRight();
        delay(turnTime);
      } else {
        turnLeft();
        delay(turnTime);
        moveForward();
        delay(sideStepTime);
        currentX += 0.5;
        turnLeft();
        delay(turnTime);
      }
      
      // Check if we're exceeding court width
      if (currentX >= COURT_WIDTH) {
        stop();
        break;
      }
    }
  }

  Serial.println("Court coverage complete");
  stop();
  while(true);
}

void moveForward() {
  motor1.setSpeed(motorSpeed);
  motor2.setSpeed(motorSpeed);
  motor3.setSpeed(motorSpeed);
  motor4.setSpeed(motorSpeed);
  
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void turnRight() {
  motor1.setSpeed(motorSpeed);
  motor2.setSpeed(motorSpeed);
  motor3.setSpeed(motorSpeed);
  motor4.setSpeed(motorSpeed);
  
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

void turnLeft() {
  motor1.setSpeed(motorSpeed);
  motor2.setSpeed(motorSpeed);
  motor3.setSpeed(motorSpeed);
  motor4.setSpeed(motorSpeed);
  
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void stop() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}


// 13.4m length x 6.1m width

// START→   Path Direction
//   ┌─────────────────────────┐
//   │  ──────────────────►    │
//   │                         │
//   │  ◄──────────────────    │
//   │                         │
//   │  ──────────────────►    │
//   │                         │
//   │  ◄──────────────────    │
//   └─────────────────────────┘

// Legend:
// ──► Forward movement
// ◄── Return movement
//  │  Side step (0.5m)