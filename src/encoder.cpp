/*                 iv.labs summer project
 *                    MOBILE ROBOT
 * ______________________________________________________
 *                    ENCODERS_CODE
 * 
 * 
 * Reads odometry data from wheel encoders and publishes as Counter type message.
 * 
 */

#include "ros/ros.h"
#include "mobile_robot/Counter.h"
#include <wiringPi.h>



int main(int argc, char** argv)
{
	//Initializing Variables
	int cntr_1 = 0, cntr_2 = 0;
	int prev_st_1 = 0, crr_st_1 = 0;
	int prev_st_2 = 0, crr_st_2 = 0;
	int flag = 0;

    //Initial setup
    ros::init(argc, argv, "encoder_node");
    ros::NodeHandle n;
    ros::Publisher chatter_pub = n.advertise<mobile_robot::Counter>("odometry", 1000);
    //ros::Rate loop_rate(100);
	wiringPiSetup();
	

	pinMode(3, INPUT);
	pinMode(4, INPUT);
	
	
	prev_st_1 = digitalRead(3);
	crr_st_1  = digitalRead(3);
	prev_st_2 = digitalRead(4);
	crr_st_2  = digitalRead(4);

      
    ROS_INFO("______STARTED ENCODER NODE_____");   
   	
   	
   	
   	 while (ros::ok())
	 {
		//Read values from sensor
		crr_st_1 = digitalRead(3);
		crr_st_2 = digitalRead(4);

		mobile_robot::Counter msg;
			
		//Check for transitions from 1 to 0 or vice-versa	
		//Check Encoder 1 	
		if(crr_st_1 != prev_st_1)
		{
			prev_st_1 = crr_st_1;
			cntr_1++;
			flag = 1;                  //Set flag high
		}
		
		//Check Encoder 2 
		if(crr_st_2 != prev_st_2)
		{
			prev_st_2 = crr_st_2;
			cntr_2++;
			flag = 1;                  //Set flag high
		}
		
		//Publish if flag is set
		if(flag == 1)
		{	
			msg.cntr_sensor_1 = cntr_1;
			msg.cntr_sensor_2 = cntr_2;
			chatter_pub.publish(msg);
			ROS_INFO("Counter = %d", cntr_1);      //Uncomment for debugging
			flag = 0;                   //Set flag low
		}
		
		
        //loop_rate.sleep();
		
	 }
	
	 return 0;
}

