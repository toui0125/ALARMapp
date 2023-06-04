from alarmer.models import alarm

import pytest


class TestAlarmRobot(object):
  def setup_method(self):
    self.test_alarm_robot = alarm.AlarmRobot()

  @pytest.mark.skip(reason = 'This method is private')
  def test_set_time(self):
    pass

  @pytest.mark.skip(reason = 'This method is private')
  def test_set_all_time(self):
    pass

  @pytest.mark.skip(reason = 'This method is private')
  def test_use_past_settings(self):
    pass

  @pytest.mark.skip(reason = 'This method is private')
  def test_fetch_past_settings(self):
    pass

  @pytest.mark.skip(reason = 'This method is private')
  def test_alarm_date_delay(self):
    pass

  @pytest.mark.skip(reason = 'This method is private')
  def test_sound(self):
    pass

  @pytest.mark.skip(reason = 'I performed black-box and white-box testing')
  def test_set_alarm(self):
    pass