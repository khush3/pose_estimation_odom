/*                 iv.labs summer project
 *                    MOBILE ROBOT
 * ______________________________________________________
 *                  POSE_ESTIMATION_CODE
 *
 *          (+ve y-axis)  ^
 *          (+ve x-axis)  <
 *          (+ve  phi  )  anti-clockwise
 */

#include "ros/ros.h"
#include <math.h>
#include <mobile_robot/Counter.h>
#include "mobile_robot/state.h"


//define some constants
#define WHEEL_DIAMETER 0.065444
#define BASE_LENGTH 1.55
#define PI 3.14159265358979

#define disp_per_tick (PI*WHEEL_DIAMETER/60.0)
#define angl_per_tick (2*disp_per_tick*BASE_LENGTH)


//cordinates of robot
float phi = 0;                     //Absolute phi for robot
float curr_disp = 0;               //diplacement for constant phi
float abs_x = 0;
float abs_y = 0;
float prev_mil = 0;
float curr_mil = 0;
int flag = 0;
char key = '0';



void pose_calc(const mobile_robot::Counter cntr_msg)
{
  //ROS_INFO("ENTERED POSE");   //uncomment for debugging


  
  if(key == 'd')
  {
	//ROS_INFO("Entering d>>");         //uncomment for debugging
    if(flag == 1)
    {
      abs_x += curr_disp*sin(phi);
      abs_y += curr_disp*cos(phi);
      flag = 0;
      prev_mil = curr_mil;
      ROS_INFO("abscissa : %f ordinate : %f | phi : %f", abs_x, abs_y, phi);
    }

    curr_mil  = ((cntr_msg.cntr_sensor_1) + (cntr_msg.cntr_sensor_2))/2.0;
    curr_disp = curr_mil - prev_mil;
    phi -= (curr_disp*angl_per_tick);
    prev_mil = curr_mil;

  }


  if(key == 'a')
  {
	  //ROS_INFO("Entering a<<");              //uncomment for debugging

    if(flag == 1)
    {
      abs_x += curr_disp*sin(phi);
      abs_y += curr_disp*cos(phi);
      flag = 0;
      prev_mil - curr_mil;
      ROS_INFO("abscissa : %f ordinate : %f | phi : %f", abs_x, abs_y, phi);
    }

    curr_mil  = ((cntr_msg.cntr_sensor_1) + (cntr_msg.cntr_sensor_2))/2.0;
    curr_disp = curr_mil - prev_mil;
    phi += (curr_disp*angl_per_tick);
    prev_mil = curr_mil;

  }

  if(key == 'w')
  {
    //ROS_INFO("Entering w");              //uncomment for debugging
    flag = 1;                    
    curr_mil = ((((cntr_msg.cntr_sensor_1) + (cntr_msg.cntr_sensor_2))/2.0) * disp_per_tick);
    curr_disp = curr_mil - prev_mil;
  }
}


void decide_state( const mobile_robot::state st_msg )
{
	//ROS_INFO("ENTERED DECIDE STATE");                   //uncomment for debugging
	if(st_msg.key == 'w')
		key = 'w';
	
	else if(st_msg.key == 'a')
		key = 'a';
	
	else if(st_msg.key == 'd')
		key = 'd';
		
}


int main(int argc, char **argv)
{
  //Initial Setups
  ros::init(argc, argv, "pose_estimator_node");       
  
  ros::NodeHandle n1;
  ros::NodeHandle n2;
  
  
  //Print to screen when node starts
  ROS_INFO("INITIALIZATIONS COMPLETED");
  
  
  ros::Subscriber sub1 = n1.subscribe("odometry", 1000, pose_calc);
  ros::Subscriber sub2 = n2.subscribe("state", 1000, decide_state);

  ros::spin();

  return 0;
}
