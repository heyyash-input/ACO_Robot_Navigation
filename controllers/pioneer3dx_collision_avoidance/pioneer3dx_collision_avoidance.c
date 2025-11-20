#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/distance_sensor.h>
#include <webots/gps.h>
#include <stdio.h>

#define TIME_STEP 64
#define MAX_SPEED 5.24

// Device tags
WbDeviceTag left_motor, right_motor, gps, sensors[16];

int main() {
  wb_robot_init();

  // Get motors
  left_motor  = wb_robot_get_device("left wheel");
  right_motor = wb_robot_get_device("right wheel");

  wb_motor_set_position(left_motor, INFINITY);
  wb_motor_set_position(right_motor, INFINITY);

  wb_motor_set_velocity(left_motor, 0);
  wb_motor_set_velocity(right_motor, 0);

  // Enable sensors so0 - so15
  char name[5];
  for(int i=0;i<16;i++){
    sprintf(name,"so%d",i);
    sensors[i] = wb_robot_get_device(name);
    wb_distance_sensor_enable(sensors[i], TIME_STEP);
  }

  // Enable GPS (if robot has it)
  gps = wb_robot_get_device("gps");
  int GPS_ON = 0;
  if(gps){
    wb_gps_enable(gps, TIME_STEP);
    GPS_ON = 1;
    printf("[+] GPS FOUND & ENABLED\n");
  } else {
    printf("[!] NO GPS FOUND — WALL AVOID LIMIT DISABLED\n");
  }

  // Arena limits
  double MIN_X=-3.3, MAX_X=3.3, MIN_Y=-3.3, MAX_Y=3.3;

  printf("[+] Controller Running...\n");

  while(wb_robot_step(TIME_STEP) != -1){

    // GPS border control
    if(GPS_ON){
      const double *p = wb_gps_get_values(gps);
      double x=p[0], y=p[2];

      if(x<MIN_X || x>MAX_X || y<MIN_Y || y>MAX_Y){
        printf("⚠️ Outside world bounds — turning back\n");
        wb_motor_set_velocity(left_motor, -0.6*MAX_SPEED);
        wb_motor_set_velocity(right_motor, 0.6*MAX_SPEED);
        continue;
      }
    }

    // Read front IR sensors
    double FL = wb_distance_sensor_get_value(sensors[0]) +
                wb_distance_sensor_get_value(sensors[1]) +
                wb_distance_sensor_get_value(sensors[2]);

    double FR = wb_distance_sensor_get_value(sensors[13]) +
                wb_distance_sensor_get_value(sensors[14]) +
                wb_distance_sensor_get_value(sensors[15]);

    double left = MAX_SPEED;
    double right = MAX_SPEED;
    double T = 100; // threshold

    // Obstacle reaction
    if(FL>T && FR>T){
      left = 0.5*MAX_SPEED;
      right = -0.5*MAX_SPEED;
    } else if(FL>T){
      left = 0.8*MAX_SPEED;
      right = -0.2*MAX_SPEED;
    } else if(FR>T){
      left = -0.2*MAX_SPEED;
      right = 0.8*MAX_SPEED;
    } else {
      left = MAX_SPEED;
      right = MAX_SPEED;
    }

    wb_motor_set_velocity(left_motor,left);
    wb_motor_set_velocity(right_motor,right);
  }

  wb_robot_cleanup();
  return 0;
}
