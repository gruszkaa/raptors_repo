#include "ros/ros.h"
#include "std_msgs/String.h"
#include "nlohmann/json.hpp"
#include <sstream>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

using json = nlohmann::json;

//metoda wyznaczajaca timestemp
std::string getCurrentTime()
{
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_c), "%T");
    return ss.str();
}


void writeToFile(const std::string &filename, const std::string &content)
{
    std::string filepath = "dump/" + filename;

    // Create the "dump" folder if it doesn't exist
    if (!boost::filesystem::exists("dump"))
    {
    boost::filesystem::create_directory("dump");
    
    }
    // Use boost::filesystem to write to the file
    std::ofstream file(filepath, std::ios::app);
    if (file.is_open())
    {
        file << "[" << getCurrentTime() << "] " << content << std::endl;
        file.close();
    }
    else
    {
        ROS_ERROR("Unable to open the file for writing.");
    }
}


//tworzenie wiadomosci json
json createJsonMessage(const std::string &originalMessage)
{
    json messageJson;
    messageJson["content"] = originalMessage;
    messageJson["signature"] = "Natalia Nykiel / " + getCurrentTime();
    return messageJson;
}


//sprawdzanie czy wiadomosc jest formatu json
bool is_json(const std::string &str)
{
  	 try
	{
    	   auto parsed_json = json::parse(str);
    	   static_cast<void>(parsed_json);  
    	   return true; 
	}
	 catch (const json::parse_error &e)
	{
    	    return false; 
	}
}

//obsluga zawartosc RaptorsIN
void raptorInCallback(const std_msgs::String::ConstPtr &msg)
{
    std::string received_message = msg->data;

    if (received_message.empty())
    {
        ROS_WARN("Received an empty message. Skipping processing.");
        return;
    }
    
    //zapisanie orginalnej wiadomosci to pliku
    writeToFile("original_messages_log.txt", received_message);


    json receivedJson;

    //spr formatu wiadomosci
   if (is_json(received_message))
   {
    receivedJson = json::parse(received_message);
    }
   else{
    receivedJson["content"] = received_message;
    }

    //modyfikacja otrzymanej wiadomosci
    json modifiedJson = createJsonMessage(receivedJson["content"]);
    
    //zapis zmodyfikowanej wiadomosci do pliku
    writeToFile("modified_messages_log.txt", modifiedJson.dump());


    // wrzucenie zmodyfikowanej wiadomosci na RaptorsOUT
    static ros::NodeHandle n;
    static ros::Publisher raptorOutPub = n.advertise<std_msgs::String>("RaptorsOut", 1000, true);

    std_msgs::String modified_msg;
    
    //konwersja json na stringa
    modified_msg.data = modifiedJson.dump(); 
    raptorOutPub.publish(modified_msg);

    ROS_INFO("Messages received and sent to RaptorsOut: [%s]", modified_msg.data.c_str());
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "RaptorsNode");
    ros::NodeHandle n;

    // Subscribe to the "RaptorsIN" topic
    ros::Subscriber raptorInSub = n.subscribe("RaptorsIN", 1000, raptorInCallback);

    ros::spin();

    return 0;
}





