/*                 iv.labs summer project
 *                    MOBILE ROBOT
 * ______________________________________________________
 *                  TELEOP_MOTOR_CONTROL_CODE
 * 
 *                            w(^)              
 *                  (<)a                s(>)
 *
 */
#include "ros/ros.h"
#include "mobile_robot/state.h"
#include <wiringPi.h>
#include <softPwm.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

#define R_MTR_PIN_FRNT 1
#define R_MTR_PIN_BKWD 23
#define L_MTR_PIN_FRNT 26
#define L_MTR_PIN_BKWD 24

#define move_speed 40
#define turn_speed 30

/* reads from keypress, doesn't echo */
int getch(void)
{
    struct termios oldattr, newattr;                     //Function for realtime keyboard controller 
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

void move_frwd(void)
{
    //ROS_INFO("HEADING ST");                  //uncomment for debugging
	  softPwmWrite(L_MTR_PIN_FRNT, move_speed);
	  softPwmWrite(L_MTR_PIN_BKWD, 00);
	  softPwmWrite(R_MTR_PIN_FRNT, move_speed);
	  softPwmWrite(R_MTR_PIN_BKWD, 00);
}

void turn_l(void)
{
    //ROS_INFO("TURNING<<");                  //uncomment for debugging
	  softPwmWrite(L_MTR_PIN_FRNT, 00);
	  softPwmWrite(L_MTR_PIN_BKWD, turn_speed);
	  softPwmWrite(R_MTR_PIN_FRNT, turn_speed);
	  softPwmWrite(R_MTR_PIN_BKWD, 00);
}

void turn_r(void)
{
  //ROS_INFO("TURNING>>");                  //uncomment for debugging
  softPwmWrite(L_MTR_PIN_FRNT, turn_speed);
  softPwmWrite(L_MTR_PIN_BKWD, 00);
  softPwmWrite(R_MTR_PIN_FRNT, 00);
  softPwmWrite(R_MTR_PIN_BKWD, turn_speed);
}

void stop(void)
{
    //ROS_INFO(">>>STOPPED<<<");                  //uncomment for debugging
	  softPwmWrite(L_MTR_PIN_FRNT, 0);
	  softPwmWrite(L_MTR_PIN_BKWD, 0);
 	  softPwmWrite(R_MTR_PIN_FRNT, 0);
	  softPwmWrite(R_MTR_PIN_BKWD, 0);
}


int main(int argc, char **argv)
{
  //Initial Setups
  ros::init(argc, argv, "teleop_motor_control_node");
  ros::NodeHandle n;
  ros::Publisher chatter_pub = n.advertise<mobile_robot::state>("state", 50);
  wiringPiSetup();

  //Declare outputs
  pinMode(L_MTR_PIN_FRNT, OUTPUT);
  pinMode(L_MTR_PIN_BKWD, OUTPUT);
  pinMode(R_MTR_PIN_FRNT, OUTPUT);
  pinMode(R_MTR_PIN_BKWD, OUTPUT);

  //Initialize pins for PWM                        //Auto Start
  softPwmCreate(L_MTR_PIN_FRNT, 0, 100);
  softPwmCreate(L_MTR_PIN_BKWD, 0, 100);
  softPwmCreate(R_MTR_PIN_FRNT, 0, 100);
  softPwmCreate(R_MTR_PIN_BKWD, 0, 100);

  ROS_INFO("__STARTED MOTOR CONTROL___");

  while(ros::ok())
  {
	//create a msg object of type state  
    mobile_robot::state s;
    
    
    s.key = getch();
    
	if(s.key == 'w')
    {
      move_frwd();
      chatter_pub.publish(s);
    }
    
    else if(s.key == 'a')
    {
      turn_l();
      chatter_pub.publish(s);
    }

    else if(s.key == 'd')
    {
      turn_r();
      chatter_pub.publish(s);
    }

    else if(s.key == 's')
    {
      stop();
      chatter_pub.publish(s);
    }
    
	else
	{
		stop();
	}
  }

  ros::spin();

  return 0;
}
