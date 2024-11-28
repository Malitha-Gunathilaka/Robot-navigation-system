# Badminton Court Robot Navigation System

An Arduino-based robotic navigation system designed to systematically traverse and cover a badminton court area. The system uses compass-based navigation (GY-271/QMC5883L) and four DC motors for precise movement control.

## Features

- Compass-guided navigation using QMC5883L sensor
- Systematic court coverage pattern
- Support for different court dimensions:
  - Full court (13.4m × 6.1m)
  - Small area (4m × 2m)
- Real-time heading correction
- Four-wheel drive system

## Hardware Requirements

- Arduino board
- Motor Shield (L293D)
- 4 DC motors
- QMC5883L Compass sensor
- Wheels and chassis
- Power supply

## Pin Configuration
// Motor initialization for layout:
//    M1    M2  (Front)
//     ┌────┐
//     │    │
//     │    │
//     └────┘
//    M4    M3  (Back)

M1: Front Left (Motor Shield Port 1)
M2: Front Right (Motor Shield Port 2)
M3: Back Right (Motor Shield Port 3)
M4: Back Left (Motor Shield Port 4)
