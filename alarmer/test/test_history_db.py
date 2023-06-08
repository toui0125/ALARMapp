from alarmer.models_db import history


import pytest
import os


class TestHistoryModel(object):
  def setup_method(self):
    self.test_history_model = history.HistoryModel()

  @pytest.mark.skip(reason = 'This method is private')
  def test_get_db_file_name(self):
    pass

  def test_load_histroy(self):
    self.test_history_model.curs.execute('insert into history values(1, "10:10", "11:11", "12:12")')
    result = self.test_history_model.load_history()
    assert result == [(1, "10:10", "11:11", "12:12")]

  def teardown_method(self):
    os.remove('history.db')