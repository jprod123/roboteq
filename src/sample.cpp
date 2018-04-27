#include <iostream>
#include <sstream>

#include <ros/ros.h>

#include <stdio.h>
#include <std_msgs/String.h>

#include <RoboteqDevice/RoboteqDevice.h>
#include <RoboteqDevice/ErrorCodes.h>
#include <RoboteqDevice/Constants.h>
#include <sensor_msgs/Joy.h>

#include <std_msgs>

using namespace std;


void roboteq_cb(sensor_msgs::Joy::ConstPtr& msg)
{
  int left = (msg.axes[0] * 1000);
  int right = (msg.axes[1] * 1000);

	device.SetCommand(_GO, 1 ,left);
	device.SetCommand(_GO, 2 , right);
}



int main(int argc, char *argv[])
{
//	
 	ros::init(argc, argv, "talker");
	ros::NodeHandle z;
	ros::Subscriber sub = z.subscriber("roboteq_cb", 1000, pub;		 
	ros::spin();

	ros::Publisher	joyPublisher publisher = n.advertise(std_msgs::String>("publisher", 1000);

	

//
	string response = "";
	RoboteqDevice device;
	int status = device.Connect("/dev/ttyACM0", 115200);

	if(status != RQ_SUCCESS)
	{
		cout<<"Error connecting to device: "<<status<<"."<<endl;
		return 1;
	}

	
	cout<<"- SetCommand(_GO, 1, 1)...";

	if((status = device.SetCommand(_GO, 1, 1)) != RQ_SUCCESS)
		cout<<"failed --> "<<status<<endl;
	else
		cout<<"succeeded."<<endl;

	device.Disconnect();
	return 0;
}
