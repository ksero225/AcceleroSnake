# AcceleroSnake

AcceleroSnake is a Snake game controlled by a real IMU sensor using ROS 2 communication on Raspberry Pi 5.

The goal of the project is to combine:
- modern C++ development,
- ROS 2 architecture,
- embedded systems,
- sensor data processing,
- and basic game development.

Instead of using a keyboard, the snake is controlled by physical movement and rotation of the sensor.

---

# Technologies

## Hardware
- Raspberry Pi 5
- Gravity BMI160 6DoF IMU
  - 3-axis accelerometer
  - 3-axis gyroscope
  - DFRobot SEN0250

## Software
- Ubuntu
- ROS 2 Jazzy
- C++
- Python

---

# Project Architecture

The project is divided into two main parts.

## 1. Sensor Node (Python)

A ROS 2 publisher node written in Python is responsible for:
- reading live data from the BMI160 sensor,
- processing accelerometer and gyroscope values,
- publishing movement data through ROS 2 topics.

Python is used here because of the availability of sensor libraries and faster hardware integration.

---

## 2. Game Node (C++)

The Snake game itself is implemented in modern C++.

The game node:
- subscribes to ROS 2 sensor topics,
- interprets movement direction,
- updates game state,
- renders the snake in terminal,
- handles collisions and score system.

The project focuses heavily on:
- object-oriented programming,
- game loop architecture,
- vector-based body simulation,
- terminal rendering,
- real-time input handling.

---

# Features

- Snake controlled by IMU movement
- ROS 2 publisher/subscriber architecture
- Dynamic snake body growth
- Collision detection
- Random food generation
- Difficulty scaling based on score
- Non-blocking terminal input
- Modular C++ architecture

---

# ROS 2 Communication

Example architecture:

```text
BMI160 Sensor
      ↓
Python Publisher Node
      ↓
ROS 2 Topic
      ↓
C++ Snake Subscriber Node
      ↓
Game Logic & Rendering
