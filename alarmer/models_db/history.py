import sqlite3


class DbModel(object):
  """DBファイルを扱うクラスの親クラス"""
  def __init__(self, db_file):
    self.db_file = db_file
    self.conn = sqlite3.connect(self.db_file)
    self.curs = self.conn.cursor()
    self.curs.execute(
      'create table if not exists history(\
        ID integer primary key autoincrement,\
        START_ALARM_TIME text,\
        STOP_ALARM_TIME text,\
        INTERVAL text\
      )'
    )
    self.conn.commit()


class HistoryModel(DbModel):
  """アラームの履歴を読み書きするクラス"""
  def __init__(self):
    db_file = self.__get_db_file_name()
    super().__init__(db_file)

  
  def __get_db_file_name(self):
    """DBファイルの名前を設定する
    
    Returns:
      db_file_name (str): DBファイルの名前
    """
    db_file_name = None

    try:
      import settings
      if settings.DB_FILE_NAME:
        db_file_name = settings.DB_FILE_NAME
    except ImportError:
      pass

    if db_file_name is None:
      db_file_name = 'history.db'

    return db_file_name


  def load_history(self):
    """DBファイルを読み込む
    
    Returns:
      rows (list): DBファイルの内容
    """
    rows = self.curs.execute('select * from history').fetchall()
    self.conn.commit()

    print('ID', end='　')
    print('START_ALARM_TIME', end='　')
    print('STOP_ALARM_TIME', end='　')
    print('INTERVAL')
    for j in range(0, len(rows)):
      print(rows[j][0], end='')
      print('　　　', rows[j][1], end='')
      print('　　　　', rows[j][2], end='')
      print('　　　', rows[j][3])

    return rows