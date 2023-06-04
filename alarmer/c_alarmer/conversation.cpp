#include "conversation.h"


void go_alarm()
{
  AlarmRobot* alarm_robot = new AlarmRobot();
  alarm_robot->set_alarm();
  delete alarm_robot;
}