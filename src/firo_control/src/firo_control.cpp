#include <combined_robot_hw/combined_robot_hw.h>
#include <controller_manager/controller_manager.h>
#include <ros/ros.h>
#include <ros/spinner.h>

int main(int argc, char** argv)
{
  ros::init(argc, argv, "firo_control");

  ros::AsyncSpinner spinner(1);
  spinner.start();
  ros::NodeHandle nh;

  // Load parameters
  double update_rate;
  nh.param("update_rate", update_rate, 25.0);

  // Initialize robot hardware and link to controller manager
  combined_robot_hw::CombinedRobotHW hw;
  bool init_success = hw.init(nh, nh);
  controller_manager::ControllerManager cm(&hw, nh);

  ROS_INFO("firo_control started");
  ros::Rate rate(update_rate);
  while (ros::ok())
  {
    hw.read(ros::Time::now(), rate.expectedCycleTime());
    cm.update(ros::Time::now(), rate.expectedCycleTime());
    hw.write(ros::Time::now(), rate.expectedCycleTime());
    rate.sleep();
  }

  spinner.stop();
  return 0;
}