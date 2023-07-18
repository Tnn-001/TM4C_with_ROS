
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/UInt32.h"

#include <stdint.h>
#include <stdio.h>

using namespace std;
#include <iostream>

#ifdef __cplusplus
extern "C"
{
#endif
#include "c_uart.h"
#ifdef __cplusplus
}
#endif

void SpeedCallback(const std_msgs::UInt32::ConstPtr &msg)
{
  ROS_INFO("I heard: [%x]", msg->data);
  uartWrite(msg->data);
}
// %EndTag(CALLBACK)%

int main(int argc, char **argv)
{
  /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line.
   * For programmatic remappings you can use a different version of init() which takes
   * remappings directly, but for most command-line programs, passing argc and argv is
   * the easiest way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
  ros::init(argc, argv, "listener");
  if(!uartInit()) return 0;
  ROS_INFO("Start");
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

  ros::Subscriber sub = n.subscribe("desire_speed", 5, SpeedCallback);
  // %EndTag(SUBSCRIBER)%
  // ros::Publisher pub = n.advertise<std_msgs::UInt32>("true_speed", 1000);
  ros::Rate loop_rate(10);
  while (ros::ok())
  {
    ros::spinOnce();
    // std_msgs::UInt32 t_speed;
    Speed s = uartRead();
    // ROS_INFO("True Speed is: [%i,%i]", s.Speed1, -1*s.Speed2);
    loop_rate.sleep();
  }
  uartClose();
  return 0;
}
