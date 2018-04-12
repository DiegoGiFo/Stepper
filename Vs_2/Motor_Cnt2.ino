#include <ros.h>
#include <StepperDriver.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Float32MultiArray.h>
#include "Motor.hpp"
#include "Robot.hpp"

#define EN 8

ros::NodeHandle  nh; // allows to create publisher/subscriber
geometry_msgs::Twist vel;
std_msgs::Float32MultiArray msg_vel;

char dim0_label[] = "velocity_mt";

Robot robot(2,5,3,6);

void motors_cb(const geometry_msgs::Twist &move)
{
  robot.set_motors(move);

  vel.linear.x = move.linear.x;
  vel.angular.z = move.angular.z;

  msg_vel.data[0] = robot.info_msgr;
  msg_vel.data[1] = robot.info_msgl;
}

ros::Subscriber<geometry_msgs::Twist> sub("/cmd_vel", &motors_cb);
ros::Publisher pub("/info_vel", &vel);
ros::Publisher pub_vel("/velocity", &msg_vel);

void setup ()
{
  //need to set the enable to LOW because if it is HIGH the motors are desabled
  pinMode(EN, OUTPUT);
  digitalWrite(EN, LOW);

  StepperDriver.init();

  nh.initNode(); // initialize ROS nodes

  msg_vel.layout.dim = (std_msgs::MultiArrayDimension *)
  malloc(sizeof(std_msgs::MultiArrayDimension)*2);
  msg_vel.data_length = 2;
  msg_vel.layout.dim[0].label = dim0_label;
  msg_vel.layout.dim[0].size = 2;
  msg_vel.layout.dim[0].stride = 1*2;
  msg_vel.layout.data_offset = 0;
  msg_vel.data = (float *)malloc(sizeof(float)*2);


  nh.advertise(pub_vel);
  nh.advertise(pub);
  nh.subscribe(sub);

}

void loop()
{
  if((millis()%50) == 1)
  {
    pub.publish(&vel);
    pub_vel.publish(&msg_vel);
  }

  robot.run_mt();

  nh.spinOnce();
}
