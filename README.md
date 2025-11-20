# 🤖 Bio-Inspired ACO-Based Robot Navigation in Webots

This project demonstrates an **Ant Colony Optimization (ACO)-inspired autonomous navigation system** built in **Webots**.  
It enables a robot to move intelligently toward a defined goal while avoiding static obstacles using real-time sensor feedback.

---

## 🚀 Features
- GPS and IMU-based goal tracking  
- LIDAR-driven obstacle detection and avoidance  
- Proportional control for smooth turns  
- Collision-free navigation  
- Easily extendable for multi-robot (swarm) environments  

---

## 🧠 Tech Stack
| Component | Technology |
|------------|-------------|
| **Programming Language** | Python 3.10+ |
| **Simulator** | Webots R2024b |
| **Robot Model** | TurtleBot3 Burger |
| **Sensors Used** | LIDAR, IMU, GPS |
| **Libraries** | `math`, `matplotlib`, `numpy` |

---

## ⚙️ Setup Instructions

### Step 1 — Clone the Repository
```bash
git clone https://github.com/heyyash-input/ACO_Robot_Navigation.git
cd ACO_Robot_Navigation

ACO_Robot_Navigation/
│── controllers/
│    └── pioneer_py/
│         └── pioneer_py.py        # Final robot controller (navigation + obstacle avoid)
│
│── worlds/
│    └── TurtleACO.wbt             # Your Webots simulation world
│
│── assets/
│    ├── diagrams/
│    ├── graphs/
│    └── robot_images/
│
│── README.md                      # Documentation (this file)
│── requirements.txt               # Python dependencies
│── LICENSE                        # MIT License
│── .gitignore                     # Ignore unwanted files
