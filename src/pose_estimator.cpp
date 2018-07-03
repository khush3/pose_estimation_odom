/*                 iv.labs summer project
 *                    MOBILE ROBOT
 * ______________________________________________________
 *                  POSE_ESTIMATION_CODE
 *
 *          (+ve y-axis)        ^
 *          (+ve x-axis)        <
 *          (+ve  phi  )  anti-clockwise            
 */

#include "ros/ros.h"
#include <math.h>
#include <mobile_robot/Counter.h>
#include "mobile_robot/state.h"


//define some constants
#define WHEEL_DIAMETER 0.073174
#define BASE_LENGTH 0.156106
#define PI 3.14159265358979

#define disp_per_tick (0.0038314176)                       //disp_per_tick = PI*WHEEL_DIAMETER/60
#define angl_per_tick (0.04908738)                  //angl_per_tick = 2*disp_per_tick/BASE_LENGTH


//cordinates of robot
double phi = 0;                     //Absolute phi for robot
//float curr_disp = 0;               //diplacement for constant phi         //uncomment if considering reverse motion
int curr_arc = 0;
float abs_x = 0;
float abs_y = 0;

float prev_mil = 0;
float curr_mil = 0;

int flag = 0;
char key = 's';



void pose_calc(const mobile_robot::Counter cntr_msg)
{

  curr_mil = (((cntr_msg.cntr_sensor_1) + (cntr_msg.cntr_sensor_2))/2.0);


  if(key == 'd')
  {
    curr_arc = (curr_mil - prev_mil);
    flag = 2;
  }


  if(key == 'a')
  {
    curr_arc = (prev_mil - curr_mil);
    flag = 2;
  }

  if(key == 'w')
  {
    flag = 1;
  }


  else if(key == 's')
  {
	  if(flag == 1)
      {
		abs_x += (curr_mil - prev_mil)*disp_per_tick*sin(phi);
		abs_y += (curr_mil - prev_mil)*disp_per_tick*cos(phi);
		flag = 0;
		prev_mil = curr_mil;
	  }

      if(flag == 2)
      {
		phi += (curr_arc * angl_per_tick);
		flag = 0;
		prev_mil = curr_mil;
      }

	  ROS_INFO("abscissa : %f ordinate : %f | phi : %lf", abs_x, abs_y, (phi*180/PI));
  }

}


void decide_state( const mobile_robot::state st_msg )
{

    if(st_msg.key == 'w')
		key = 'w';

	else if(st_msg.key == 'a')
		key = 'a';

	else if(st_msg.key == 'd')
		key = 'd';

    else if(st_msg.key == 's')
		key = 's';
}


int main(int argc, char **argv)
{
  //Initial Setups
  ros::init(argc, argv, "pose_estimator_node");

  ros::NodeHandle n1;
  ros::NodeHandle n2;

  ROS_INFO("INITIALIZATIONS COMPLETED");


  ros::Subscriber sub1 = n1.subscribe("odometry", 50, pose_calc);
  ros::Subscriber sub2 = n2.subscribe("state", 10, decide_state);

  ros::spin();

  return 0;
}
