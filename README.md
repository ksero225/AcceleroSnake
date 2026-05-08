# AcceleroSnake

AcceleroSnake is a Snake game controlled by a real IMU sensor using ROS 2 communication on Raspberry Pi 5.

The project combines:
- modern C++ development,
- ROS 2 publisher/subscriber architecture,
- embedded systems,
- I2C sensor communication,
- accelerometer-based input control,
- and basic terminal game development.

Instead of using a keyboard, the snake is controlled by physically tilting the BMI160 IMU sensor.

---

# Project Status

Current version:
- BMI160 IMU sensor is read through I2C,
- Python ROS 2 node publishes movement directions,
- C++ Snake game subscribes to direction commands,
- Snake can be controlled using physical IMU movement,
- terminal-based rendering and score system are implemented.

The current control mode uses accelerometer-based tilt detection.  
The gyroscope is available for raw readings and may be used in future control modes.

---

# Technologies

## Hardware

- Raspberry Pi 5
- Gravity BMI160 6DoF IMU - DFRobot SEN0250
  - 3-axis accelerometer
  - 3-axis gyroscope

## Software

- Ubuntu
- ROS 2 Jazzy
- C++
- Python
- I2C
- colcon

---

# Project Architecture

The project is divided into two ROS 2 packages:

```text
src/
├── snake/          # C++ Snake game node
└── imu_reader/     # Python BMI160 IMU reader and direction publisher
```

The `snake` package contains the C++ terminal Snake game.

The `imu_reader` package contains Python nodes responsible for reading BMI160 sensor data and publishing movement directions through ROS 2 topics.

---

# ROS 2 Communication

The BMI160 sensor is read by a Python ROS 2 node.

The node converts accelerometer values into movement directions and publishes them to a ROS 2 topic.

```text
BMI160 Sensor
      ↓
Python IMU Publisher Node
      ↓
/snake/direction
      ↓
C++ Snake Game Node
      ↓
Game Logic & Terminal Rendering
```

---

## Topic

```text
/snake/direction
```

## Message Type

```text
std_msgs/msg/String
```

## Supported Values

```text
UP
DOWN
LEFT
RIGHT
```

---

# Features

- Snake controlled by BMI160 IMU movement
- ROS 2 publisher/subscriber communication
- Python sensor node
- C++ game node
- Terminal-based Snake rendering
- Dynamic snake body growth
- Random food generation
- Collision detection
- Score system
- Difficulty scaling based on score
- Non-blocking keyboard input support
- Modular project structure

- ---

# Requirements

Install ROS 2 Jazzy before building this project.

Install required system packages:

```bash
sudo apt update
sudo apt install python3-smbus i2c-tools
```

Check if the BMI160 sensor is visible on the I2C bus:

```bash
i2cdetect -y 1
```

Expected address:

```text
0x68 or 0x69
```

If the detected address is different from the one used in the Python code, update the `BMI160_ADDR` value in the IMU publisher file.
