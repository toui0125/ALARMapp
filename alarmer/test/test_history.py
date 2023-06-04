from alarmer.models import history

import pytest
import os


class TestHistoryModel(object):
  def setup_method(self):
    self.test_history_model = history.HistoryModel()

  @pytest.mark.skip(reason = 'This method is private')
  def test_get_csv_file_name(self):
    pass

  def test_save_and_load_history_exclude_header(self):
    self.test_history_model.save([
      ['00:00:00','01:01:01','02:02:02'],
      ['03:03:03','04:04:04','05:05:05'],
    ])
    result = self.test_history_model.load_history_exclude_header()
    assert result == [
      ['00:00:00','01:01:01','02:02:02'],
      ['03:03:03','04:04:04','05:05:05'],
    ]

  def test_save_and_load_history_include_header(self):
    self.test_history_model.save([
      ['00:00:00','01:01:01','02:02:02'],
      ['03:03:03','04:04:04','05:05:05'],
    ])
    result = self.test_history_model.load_history_include_header()
    assert result == [
      ['ID','START_ALARM_TIME','STOP_ALARM_TIME','INTERVAL'],
      ['1','00:00:00','01:01:01','02:02:02'],
      ['2','03:03:03','04:04:04','05:05:05'],
    ]

  def teardown_method(self):
    os.remove('history.csv')