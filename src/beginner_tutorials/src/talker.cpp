#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include <chrono>
#include <thread>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "talker");
  ros::NodeHandle n;

  std::this_thread::sleep_for(std::chrono::seconds(2));

  ros::Publisher raptorInPub = n.advertise<std_msgs::String>("RaptorsIN", 1000);

  ros::Rate loop_rate(5);

  int count = 0;
  while (ros::ok())
  {
    std_msgs::String msg;

    std::stringstream ss;
    ss << "wiadomosc testowa " << count;
    msg.data = ss.str();

    ROS_INFO("%s", msg.data.c_str());

    raptorInPub.publish(msg);

    ros::spinOnce();

    loop_rate.sleep();
    ++count;
  }

  return 0;
}

/*
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "nlohmann/json.hpp"
#include <sstream>
#include <chrono>
#include <thread>

using json = nlohmann::json;



json createJsonMessage(const std::string &originalMessage)
{
    json messageJson;
    messageJson["content"] = originalMessage;
    return messageJson;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "talker");
    ros::NodeHandle n;

    std::this_thread::sleep_for(std::chrono::seconds(2));

    ros::Publisher raptorInPub = n.advertise<std_msgs::String>("RaptorsIN", 1000);

    ros::Rate loop_rate(5);

    int count = 0;
    while (ros::ok())
    {
        std_msgs::String msg;

        std::stringstream ss;
        ss << "test1 " << count;

        json jsonMessage = createJsonMessage(ss.str());

        msg.data = jsonMessage.dump(); // Convert JSON to string
        ROS_INFO("%s", msg.data.c_str());

        raptorInPub.publish(msg);

        ros::spinOnce();

        loop_rate.sleep();
        ++count;
    }

    return 0;
}

*/
