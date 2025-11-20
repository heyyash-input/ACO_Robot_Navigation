# Usage Guide

## Running the Simulation

1. Launch Webots
2. Load the world file: `worlds/warehouse.wbt`
3. Start simulation
4. The robot will:
   - Localize using GPS/IMU
   - Scan environment using LIDAR
   - Navigate toward goal
   - Avoid obstacles dynamically

## Modifying the Goal
Edit in `pioneer_py.py`:

GOAL = (-2.72, 0.15)
