#include "ros/ros.h"
#include "std_msgs/String.h"

void raptorOutCallback(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("I heard from RaptorsOut: [%s]", msg->data.c_str());
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");
  ros::NodeHandle n;

  ros::Subscriber raptorOutSub = n.subscribe("RaptorsOut", 1000, raptorOutCallback);

  ros::spin();

  return 0;
}



