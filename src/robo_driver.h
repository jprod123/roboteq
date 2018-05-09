#ifndef ROBO_DRIVER_H
#define ROBO_DRIVER_H
#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <std_srvs/Empty.h>
#include <RoboteqDevice/RoboteqDevice.h>
#include <RoboteqDevice/ErrorCodes.h>
#include <RoboteqDevice/Constants.h>
#include <vector>
struct tank_steering{
  double left;
  double right;
};

class robo_driver{
  public:
    robo_driver();
    ~robo_driver();
  private:
    ros::NodeHandle nh_;
    ros::Subscriber command_sub_;
    ros::ServiceServer e_stop_service_;

    void joy_cb(const sensor_msgs::Joy &msg);
    void e_stop_cb(std_srvs::Empty::Request& req, std_srvs::Empty::Response& resp);
    tank_steering convert_to_tank_steering(double, double);

    std::vector<RoboteqDevice*> devices_;
    std::vector<std::string> port_names_;
    int number_of_devices_;
    bool has_devices_ = false;
};
#endif
