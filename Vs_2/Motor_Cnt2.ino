#include <ros.h>
#include <StepperDriver.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Float32MultiArray.h>
#include "Motor.hpp"
#include "Robot.hpp"

#define EN 8

ros::NodeHandle  nh; // allows to create publisher/subscriber
geometry_msgs::Twist vel;
std_msgs::Float32 msg;
std_msgs::Float32MultiArray arr;

Robot robot(2,5,3,6);

void motors_cb(const geometry_msgs::Twist &move)
{
  robot.set_motors(move);

  vel.linear.x = move.linear.x;
  vel.angular.z = move.angular.z;

  msg.data = robot.info_msgr;

  //arr.data[0] = robot.info_msgr;
  //arr.data[1] = robot.info_msgl;

}

ros::Subscriber<geometry_msgs::Twist> sub("/cmd_vel", &motors_cb);
ros::Publisher pub("/info_vel", &vel);
ros::Publisher pubtest("/test_speed", &msg);

void setup ()
{
  //need to set the enable to LOW because if it is HIGH the motors are desabled
  pinMode(EN, OUTPUT);
  digitalWrite(EN, LOW);

  StepperDriver.init();

  nh.initNode(); // initialize ROS nodes

  /*nh.getHardware()->setBaud(115200);
  arr.layout.dim = (std_msgs::MultiArrayDimension *)
  malloc(sizeof(std_msgs::MultiArrayDimension)*2);
  arr.layout.dim[0].label = "Info velocity";
  arr.layout.dim[0].size = 2;
  arr.layout.dim[0].stride = 1;
  arr.layout.data_offset = 0;
  arr.data = (float *) malloc(4*sizeof(float));
  arr.data_length = 4;
  */

  nh.advertise(pubtest);
  nh.subscribe(sub);
  nh.advertise(pub);
}

void loop()
{
  if((millis()%50) == 1)
  {
    pub.publish(&vel);
    pubtest.publish(&msg);
  }

  robot.run_mt();

  nh.spinOnce();
}
