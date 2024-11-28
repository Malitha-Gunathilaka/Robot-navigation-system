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
```
 Motor initialization for layout:
    M1    M2  (Front)
     ┌────┐
     │    │
     │    │
     └────┘
    M4    M3  (Back)

M1: Front Left (Motor Shield Port 1)
M2: Front Right (Motor Shield Port 2)
M3: Back Right (Motor Shield Port 3)
M4: Back Left (Motor Shield Port 4)
```

## Libraries Required

- AFMotor.h
- Wire.h
- QMC5883LCompass.h

## Movement Patterns

The robot follows a systematic pattern to cover the entire court:
1. Forward movement along court length
2. 90° turn
3. Side step (0.5m)
4. 90° turn
5. Return movement
6. Repeat until full width is covered

## Files Description

- `GY-271 compass sensor.ino`: Main implementation with compass-guided navigation
- `13.4m.ino`: Full badminton court coverage program
- `4m_2m_squar.ino`: Smaller area coverage program

## Setup and Calibration

1. Connect hardware components
2. Upload code to Arduino
3. Place robot at starting position
4. System will auto-calibrate compass on startup
5. Robot will begin systematic movement pattern

## License

MIT License

## Contributors

[Malitha-Gunathilaka]

## Notes

- Adjust `motorSpeed` and timing parameters based on your robot's specifications
- Ensure proper compass calibration before operation
- Battery voltage affects motor performance
