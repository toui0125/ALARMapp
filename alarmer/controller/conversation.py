from alarmer.models import alarm

def go_alarm():
  alarm_robot = alarm.AlarmRobot()
  alarm_robot.set_alarm()
  del alarm_robot