#include "ros/ros.h"
#include "geometry_msgs/WrenchStamped.h"
#include <iostream>  // for cout
#include <math.h>    // for sqrt
#include <stdio.h>   // for printf()
#include <queue>
#include <map>



/*
 * class for integration
 */
class Calc
{
  // 各種定数
  static const int q_size = 3;  // 積分区間分割数

  // 各種変数
  std::map< double, std::vector<double> > ee_force;
  std::queue<geometry_msgs::WrenchStamped::ConstPtr> q_f;//save latest time stamp
public:
  // 値の更新
  Calc();
  double newValue (const geometry_msgs::WrenchStamped::ConstPtr& msg);
};
Calc::Calc(){};
double Calc::newValue(const geometry_msgs::WrenchStamped::ConstPtr& msg){
  q_f.push(msg);
  std::cout << "time:"  << q_f.front()->header.seq << std::endl;
  std::cout << "que size:"  << q_f.size() << std::endl;
  if (q_f.size() > 2) q_f.pop();
};


Calc cal;
/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */
void chatterCallback(const geometry_msgs::WrenchStamped::ConstPtr& msg)
{
  // ROS_INFO("I heard: [%f]", msg->wrench.force.x);
  cal.newValue(msg);
}

int main(int argc, char **argv)
{
  /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line. For programmatic
   * remappings you can use a different version of init() which takes remappings
   * directly, but for most command-line programs, passing argc and argv is the easiest
   * way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
  ros::init(argc, argv, "force_integrater");

  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle n;

  /**
   * The subscribe() call is how you tell ROS that you want to receive messages
   * on a given topic.  This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing.  Messages are passed to a callback function, here
   * called chatterCallback.  subscribe() returns a Subscriber object that you
   * must hold on to until you want to unsubscribe.  When all copies of the Subscriber
   * object go out of scope, this callback will automatically be unsubscribed from
   * this topic.
   *
   * The second parameter to the subscribe() function is the size of the message
   * queue.  If messages are arriving faster than they are being processed, this
   * is the number of messages that will be buffered up before beginning to throw
   * away the oldest ones.
   */
  ros::Subscriber sub = n.subscribe("rhsensor", 1000, chatterCallback);

  /**
   * ros::spin() will enter a loop, pumping callbacks.  With this version, all
   * callbacks will be called from within this thread (the main one).  ros::spin()
   * will exit when Ctrl-C is pressed, or the node is shutdown by the master.
   */
  ros::spin();

  return 0;
}


