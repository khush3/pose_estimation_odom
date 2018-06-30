
/*                 iv.labs summer project
 *                    MOBILE ROBOT
 * ______________________________________________________
 *                  MOTOR_CONTROL_CODE
 *                  (for UMBmark test)
 *
 */
#include "ros/ros.h"
#include "mobile_robot/Counter.h"
#include <wiringPi.h>
#include <softPwm.h>


#define WHEEL_DIAMETER_L 0.065444
#define WHEEL_DIAMETER_R 0.067444
#define BASE_LENGTH 1.55
#define PI 3.14159265358979


#define R_MTR_PIN_FRNT 1
#define R_MTR_PIN_BKWD 23
#define L_MTR_PIN_FRNT 26
#define L_MTR_PIN_BKWD 24




void move_frwd(void)
{
      ROS_INFO("HEADING ST");                  //uncomment for debugging
	  softPwmWrite(L_MTR_PIN_FRNT,18);
	  softPwmWrite(L_MTR_PIN_BKWD, 00);
	  softPwmWrite(R_MTR_PIN_FRNT,18);
	  softPwmWrite(R_MTR_PIN_BKWD, 00);
}	  


void turn(void)
{
      ROS_INFO("TURNING>>");                  //uncomment for debugging
	  softPwmWrite(L_MTR_PIN_FRNT, 00);
	  softPwmWrite(L_MTR_PIN_BKWD, 17);
	  softPwmWrite(R_MTR_PIN_FRNT, 17);
	  softPwmWrite(R_MTR_PIN_BKWD, 00);
}

void stop(void)
{
      ROS_INFO(">>>STOPPED<<<");                  //uncomment for debugging
	  softPwmWrite(L_MTR_PIN_FRNT, 0);
	  softPwmWrite(L_MTR_PIN_BKWD, 0);
	 softPwmWrite(R_MTR_PIN_FRNT, 0);
	  softPwmWrite(R_MTR_PIN_BKWD, 0);
}
		
void motor_control(const mobile_robot::Counter msg)
{
  double motor_r_dist = (WHEEL_DIAMETER_L * msg.cntr_sensor_1 * PI)/ 60.0;
  double motor_l_dist = (WHEEL_DIAMETER_R * msg.cntr_sensor_2 * PI)/ 60.0;
  double motor_dist = (motor_l_dist + motor_r_dist)/2;

  ROS_INFO("MOTOR_RIGHT = %fm ", motor_l_dist);

  if(motor_dist <= 1.0)
  {
	  move_frwd();
  }
  else if(motor_dist >1.0  && motor_dist <= 1.11)
  {
	  turn();
  }
  else if(motor_dist >1.11 && motor_dist <= 2.11)
  {
	  move_frwd();
  }
  else if(motor_dist > 2.11 && motor_dist <= 2.22)
  {
	  turn();
  }
  else if(motor_dist > 2.22 && motor_dist <= 3.22)
  {
	  move_frwd();
  }
  else if(motor_dist > 3.22 && motor_dist <= 3.33)
  {
	  turn();
  }
  else if(motor_dist > 3.33 && motor_dist <= 4.33)
  {
	  move_frwd();
  }
  
  else
  {
	  stop();
  }	  
  
}

int main(int argc, char **argv)
{
  //Initial Setups
  ros::init(argc, argv, "motor_control_node");
  ros::NodeHandle n;
  wiringPiSetup();

  
  //Declare outputs
  pinMode(L_MTR_PIN_FRNT, OUTPUT);
  pinMode(L_MTR_PIN_BKWD, OUTPUT);
  pinMode(R_MTR_PIN_FRNT, OUTPUT);
  pinMode(R_MTR_PIN_BKWD, OUTPUT);

  //Initialize pins for PWM
  softPwmCreate(L_MTR_PIN_FRNT,18,100);
  softPwmCreate(L_MTR_PIN_BKWD, 0, 100);
  softPwmCreate(R_MTR_PIN_FRNT,18,100);
  softPwmCreate(R_MTR_PIN_BKWD, 0, 100);

  ROS_INFO("__STARTED MOTOR CONTROL___");
  
               //Auto Start

  ros::Subscriber sub = n.subscribe("odometry", 1000, motor_control);

  ros::spin();

  return 0;
}
