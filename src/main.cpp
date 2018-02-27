#include <ros/ros.h>

#include <sensor_msgs/JointState.h>
#include <geometry_msgs/Vector3.h>
#include <std_msgs/Float64.h>

ros::Publisher armLeft;
ros::Publisher armRight;
ros::Publisher waistPitch;

void onJointState(const sensor_msgs::JointState::ConstPtr &msg)
{
  geometry_msgs::Vector3 left;
  geometry_msgs::Vector3 right;
  std_msgs::Float64 waist;
  for(unsigned i = 0; i < msg->name.size(); ++i) {
    const auto &name = msg->name[i];
    const auto &position = msg->position[i];

    if(name == "Left Arm Roll")
    {
      left.x = position;
    }
    else if(name == "Left Arm Pitch")
    {
      left.y = position;
    }
    else if(name == "Right Arm Roll")
    {
      right.x = position;
    }
    else if(name == "Right Arm Pitch")
    {
      right.y = position;
    }
    else if(name == "Waist Pitch")
    {
      waist.data = position;
    }
  }

  armLeft.publish(left);
  armRight.publish(right);
  waistPitch.publish(waist);
}

int main(int argc, char *argv[])
{
  ros::init(argc, argv, "quori_joint_state_bind");

  ros::NodeHandle nh;

  armLeft = nh.advertise<geometry_msgs::Vector3>("/quori/arm_left/cmd_pos", 5, false);
  armRight = nh.advertise<geometry_msgs::Vector3>("/quori/arm_right/cmd_pos", 5, false);
  waistPitch = nh.advertise<std_msgs::Float64>("/quori/waist/cmd_pos", 5, false);

  auto sub = nh.subscribe<sensor_msgs::JointState>("/joint_states", 5, &onJointState);

  ros::spin();
}