#include <iostream>
#include <sstream>

#include <ros/ros.h>

#include <stdio.h>
#include <std_msgs/String.h>

#include <RoboteqDevice/RoboteqDevice.h>
#include <RoboteqDevice/ErrorCodes.h>
#include <RoboteqDevice/Constants.h>
#include <sensor_msgs/Joy.h>

using namespace std;


	RoboteqDevice drive1;
	RoboteqDevice drive2;
	RoboteqDevice dig;
	RoboteqDevice dump;
void cb(sensor_msgs::Joy msg)
{
  int left = (msg.axes[0] * 1000);
  int right = (msg.axes[1] * 1000);

	drive1.SetCommand(_GO, 1 ,left);
	drive1.SetCommand(_GO, 2 , right);
	drive2.SetCommand(_GO, 1 ,left);
	drive2.SetCommand(_GO, 2 , right);

  dig.SetCommand(_GO, 1, msg.axes[3] * 1000);
  dig.SetCommand(_GO, 2, msg.axes[4] * 1000);

  if (msg.buttons[5])
    dump.SetCommand(_GO, 2, 1000);
  if (msg.buttons[4])
    dump.SetCommand(_GO, 2 , -1000);

  int deploy;
  if(msg.axes[2] != 1){
    deploy = (msg.axes[2] - 1) * 500;
  } else {
    deploy = (msg.axes[5] + 1) * 500;
}
dump.SetCommand(_GO, 1, deploy); 
}

int main(int argc, char *argv[])
{
//	
 	ros::init(argc, argv, "talker");
	ros::NodeHandle z;
//
	string response = "";

	int drive1_status = drive1.Connect("/dev/ttyACM0");//, 115200);
	int drive2_status = drive2.Connect("/dev/ttyACM1");//, 115200);
	int dig_status = dig.Connect("/dev/ttyACM2");//, 115200);
	int dump_status = dump.Connect("/dev/ttyACM3");//, 115200);

	if(drive1_status != RQ_SUCCESS)
	{
		cout<<"Error connecting to drive1"<<endl;
		return 1;
	}
	if(drive2_status != RQ_SUCCESS)
	{
		cout<<"Error connecting to drive2: "<<endl;
		return 1;
	}
	if(dig_status != RQ_SUCCESS)
	{
		cout<<"Error connecting to dig:"<<endl;
		return 1;
	}
	if(dump_status != RQ_SUCCESS)
	{
		cout<<"Error connecting to dump: "<<endl;
		return 1;
	}

	ros::Subscriber sub = z.subscribe("roboteq_cb",10, cb);

  while(ros::ok()){
    ros::spinOnce();
  }
	return 0;
}
