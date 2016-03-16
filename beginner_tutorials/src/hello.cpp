# include <ros/ros.h>
# include <unistd.h>

int main (int argc,char **argv){
  //initialize the ros system
  ros::init(argc,argv,"hello_ros");
  //establish this prgram as ros node
  ros::NodeHandle nh;
  //send some output as a log message
  ROS_INFO_STREAM("Hello, ROS!");
  sleep(5);
}

