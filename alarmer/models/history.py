import csv
import pathlib
import os


HISTORY_COLUMN_ID = 'ID'
HISTORY_COLUMN_START_ALARM_TIME = 'START_ALARM_TIME'
HISTORY_COLUMN_STOP_ALARM_TIME = 'STOP_ALARM_TIME'
HISTORY_COLUMN_INTERVAL = 'INTERVAL'


class CsvModel(object):
  """CSVファイルを扱うクラスの親クラス"""
  def __init__(self, csv_file):
    self.csv_file = csv_file
    if not os.path.exists(self.csv_file):
      pathlib.Path(self.csv_file).touch()


class HistoryModel(CsvModel):
  """アラームの履歴を読み書きするクラス"""
  def __init__(self):
    csv_file = self._get_csv_file_name()
    super().__init__(csv_file)
    self.history_data = []

  
  def _get_csv_file_name(self):
    """CSVファイルの名前を設定する
    
    Returns:
      csv_file_name (str): CSVファイルの名前
    """
    csv_file_name = None

    try:
      import settings
      if settings.CSV_FILE_NAME:
        csv_file_name = settings.CSV_FILE_NAME
    except ImportError:
      pass

    if csv_file_name is None:
      csv_file_name = 'history.csv'

    return csv_file_name
  

  def load_history_exclude_header(self):
    """CSVファイルを読み込む（ヘッダー意外）。
        
    Returns:
       self.history_data (list): CSVファイルの内容（ヘッダー意外）
    """
    with open(self.csv_file, mode = 'r') as csv_file:
      reader = csv.DictReader(csv_file)

      for row in reader:
        history_data_elements = [
          row[HISTORY_COLUMN_START_ALARM_TIME],
          row[HISTORY_COLUMN_STOP_ALARM_TIME],
          row[HISTORY_COLUMN_INTERVAL]
        ]
        self.history_data.append(history_data_elements)

    return self.history_data


  def load_history_include_header(self):
    """CSVファイルを読み込む（ヘッダーも）
    
    Returns:
      rows (list): CSVファイルの内容（ヘッダーも）
    """
    with open(self.csv_file, mode = 'r') as csv_file:
      reader = csv.reader(csv_file)
      rows = [row for row in reader]

    for row in rows[0]:
      print(row, end='　')
    print()
    for j in range(1, len(rows)):
      print(rows[j][0], end='')
      print('　　　', rows[j][1], end='')
      print('　　　　', rows[j][2], end='')
      print('　　　', rows[j][3])

    return rows


  def save(self, history_data):
    """CSVファイルに書き込む
    
    Args:
      history_data (list): アラームの履歴
    """
    csv_file_column = [
      HISTORY_COLUMN_ID,
      HISTORY_COLUMN_START_ALARM_TIME,
      HISTORY_COLUMN_STOP_ALARM_TIME,
      HISTORY_COLUMN_INTERVAL
    ]
    with open(self.csv_file, mode='w') as csv_file:
      writer = csv.DictWriter(csv_file, fieldnames=csv_file_column)
      writer.writeheader()
      for id, data in zip(range(1, len(history_data)+1), history_data):
        writer.writerow({
          HISTORY_COLUMN_ID : id,
          HISTORY_COLUMN_START_ALARM_TIME: data[0],
          HISTORY_COLUMN_STOP_ALARM_TIME : data[1],
          HISTORY_COLUMN_INTERVAL : data[2]
        })