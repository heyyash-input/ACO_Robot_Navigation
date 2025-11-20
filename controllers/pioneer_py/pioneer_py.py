from controller import Robot, Lidar, InertialUnit
import math

robot = Robot()
TIME_STEP = 64
MAX_SPEED = 6.5

GOAL = (-2.72, 0.15)
GOAL_TOL = 0.25

gps = robot.getDevice("gps"); gps.enable(TIME_STEP)
imu = robot.getDevice("imu"); imu.enable(TIME_STEP)
lidar = robot.getDevice("LDS-01"); lidar.enable(TIME_STEP)

left = robot.getDevice("left wheel motor")
right = robot.getDevice("right wheel motor")
left.setPosition(float("inf")); right.setPosition(float("inf"))

def clamp(v): 
    return max(min(v, MAX_SPEED), -MAX_SPEED)

def go(l, r):
    left.setVelocity(clamp(l))
    right.setVelocity(clamp(r))

def norm(a):
    return math.atan2(math.sin(a), math.cos(a))

print("✅ Enhanced Navigation: Goal + Obstacle Avoid")

stuck_timer = 0

while robot.step(TIME_STEP) != -1:
    x, _, z = gps.getValues()
    yaw = norm(imu.getRollPitchYaw()[2])
    
    gx, gz = GOAL
    dx, dz = gx - x, gz - z
    dist = math.sqrt(dx*dx + dz*dz)
    target = math.atan2(dz, dx)
    error = norm(target - yaw)

    scan = lidar.getRangeImage()

    # ✅ **WIDER field for front**
    front = min(scan[140:220])
    left_area = min(scan[220:300])
    right_area = min(scan[60:140])

    # ✅ stop when reached goal
    if dist < GOAL_TOL:
        go(0,0)
        print("🎯 Goal Reached Successfully!")
        break

    # ✅ Emergency turn if obstacle very close
    if front < 0.45:
        stuck_timer += 1
        if right_area > left_area:
            go(-0.5 * MAX_SPEED, 0.9 * MAX_SPEED)
        else:
            go(0.9 * MAX_SPEED, -0.5 * MAX_SPEED)
        continue

    # ✅ Normal obstacle avoidance
    if front < 0.8:
        # Turn smoothly based on left/right clearance
        turn_strength = (left_area - right_area) * 2.0
        go(2.5 - turn_strength, 2.5 + turn_strength)
        continue

    # ✅ DRIVING TOWARD GOAL
    # Smooth proportional control
    turn = 1.0 * error
    fwd = 4.5

    go(fwd - turn, fwd + turn)
