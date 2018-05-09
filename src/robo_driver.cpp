#include <robo_driver.h>
#include <cmath>

robo_driver::robo_driver():
  nh_("~"),
  command_sub_(nh_.subscribe("/joy", 1, &robo_driver::joy_cb, this)),
  e_stop_service_(nh_.advertiseService("/estop", &robo_driver::e_stop_cb, this))
{
  if (!nh_.getParam("port_names", port_names)){
    ROS_ERROR("Failed to get port names!, exiting");
    ros::shutdown();
  }
  number_of_devices_ = port_names_.length();
  for (int i = 0, i < number_of_devices_, i++){
    devices_.push_back(new RoboteqDevice());
    if (devices_[i]->Connect(port_names_[i]) != RQ_SUCCESS){
      ROS_ERROR("Failed to initiate device " << port_names_[i]);
    } else {
      ROS_INFO("Device initiated at " << port_names_[i]);
      has_devices_ = true;
    }
  }
  if (!has_devices){
    ROS_ERROR("No Devices initialized, exiting");
    ros::shutdown();
  }
}
robo_driver::joy_cb(const sensor_msgs::Joy &msg){
  //The first two axes are reserved for driving as are the first two devices
  tank_steering tank = convert_to_tank_steering(msg.axes[0],msg.axes[1]);
  int j = 2; //Message offset for the rest of the devices
  for (int i = 0, i < number_of_devices, i++){
    if (i <=1){ //The first two devices are for driving
      devices_[i]->SetCommand(_GO, 1, scale(tank.left));
      devices_[i]->SetCommand(_GO, 2, scale(tank.right));
    } else {
      devices_[i]->SetCommand(_GO, 1, scale(msg.axes[j]));
      devices_[i]->SetCommand(_GO, 2 , scale(msg.axes[j+1]));
      j = j + 2; //Each devices get two axes
    }
  }
}
robo_driver::convert_to_tank_steering(double axis1, double axis2){
  tank_steering converted;
  int v = (1 - abs(axis1)) * axis2 + axis2;
  int w = (1 - abs(axis2)) * axis1 + axis1;
  converted.right = (v+w)/2;
  converted.left = (v-w)/2;
  return converted
}
robo_driver::scale(double x){
  return int(x * 1000);
}
robo_driver::e_stop_cb(std_srvs::Empty::Request& req, std_srvs::Empty::Response& resp){
  for (int i = 0, i < number_of_devices, i++){
    devices_[i]->SetCommand(_GO, 1 , 0);
    devices_[i]->SetCommand(_GO, 2 , 0);
  }
  ros::shutdown();
}
