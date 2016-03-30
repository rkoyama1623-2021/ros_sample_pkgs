#include "ros/ros.h"
#include "beginner_tutorials/AddTwoInts.h"

//request から responseを返すサービスの実態
bool add(beginner_tutorials::AddTwoInts::Request  &req,
         beginner_tutorials::AddTwoInts::Response &res)
{
  res.sum = req.a + req.b;
  ROS_INFO("request: x=%ld, y=%ld", (long int)req.a, (long int)req.b);
  ROS_INFO("sending back response: [%ld]", (long int)res.sum);
  return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "add_two_ints_server");//node name
  ros::NodeHandle n;//node handler

  ros::ServiceServer service = n.advertiseService("add_two_ints", add);//service name
  ROS_INFO("Ready to add two ints.");
  ros::spin();

  return 0;
}
