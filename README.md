# Experimental Flight Computer

This project is a Software impletementation of a flight computer for the ARC-S flight computer.
## Features

- Reads serial data from a connected device
- Real-time plotting of sensor data:
  - Magnetometer: MX, MY, MZ
  - Gyroscope: GX, GY, GZ
- Dynamic X-axis scrolling to show the latest data
- Configurable time window and buffer size
- Handles non-numeric serial lines errors
## Future
  - Replace Serial with LoRa interface
  - Add GPS
  - Add IMU
  - Add kalman filter
  - Euler to quaternarian formulas

## Requirements

- Python 3.10+
- [DearPyGui](https://github.com/hoffstadt/DearPyGui)
- PySerial

## Installation

1. Clone the repository:
   
   git clone <repository-url>

    Install dependencies:

    pip install dearpygui pyserial

Usage

    Connect your sensor device to the correct serial port.

    Update the port variable in GroundStationGUI.py if necessary:

ser = serial.Serial(
    port='/dev/cu.usbserial-0001',
    baudrate=115200,
    timeout=1
)

Run the GUI:

python GroundStationGUI.py

The windows will display real-time plots of (C++ output/filtered) sensor data.
