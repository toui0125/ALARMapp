from alarmer.views import console
from alarmer.models import history


import pygame
import time
import datetime
from datetime import datetime as dt


DEFAULT_SOUND_VOLUME = 1.0
DEFAULT_SPEAK_COLOR = 'cyan'
DEFAULT_ERROR_SPEAK_COLOR = 'magenta'


class Robot(object):
  """ロボットを生成するクラスの親クラス"""
  def __init__(self, speak_color = DEFAULT_SPEAK_COLOR,
  error_speak_color = DEFAULT_ERROR_SPEAK_COLOR):
    self.speak_color = speak_color
    self.error_speak_color = error_speak_color


class AlarmRobot(Robot):
  """アラームデータを扱うクラス"""
  def __init__(self, speak_color = DEFAULT_SPEAK_COLOR,
  error_speak_color = DEFAULT_ERROR_SPEAK_COLOR):
    super().__init__(speak_color, error_speak_color)
    self.start_alarm_time = datetime.time(0,0)
    self.stop_alarm_time = datetime.time(0,0)
    self.interval = datetime.timedelta(minutes=0)
    self.history_model = history.HistoryModel()
  

  def __del__(self):
    pygame.quit()
    del self.history_model


  def __set_time(self, time_message, time_file, min_time, max_time):
    """アラームの「時」や「分」を設定する
        
      Args:
        time_message (str): テキストファイルの内容を書き換えるための文
        time_file (str): hour.txtかminute.txt
        min_time (int): テキストファイルの内容を書き換えるための数字、
          ユーザーが入力できる「時」や「分」の最小値
        max_time (int): テキストファイルの内容を書き換えるための数字、
          ユーザーが入力できる「時」や「分」の最大値
          
      Returns:
        time (int): アラームの「時」や「分」
    """
    while True:
      message = console.make_message_from_document(
        time_file, self.speak_color,
        sub_what_do = time_message,
        sub_min_time = min_time, sub_max_time = max_time
      )
      time = input(message)

      try:
        time = int(time)
      except ValueError:
        message = console.make_message_from_document(
          'error_message.txt', self.error_speak_color,
          sub_error_message = f'{min_time}から{max_time}の整数を'
        )
        print(message)
        continue

      if min_time <= time and time <=max_time:
        break
      else:
        message = console.make_message_from_document(
          'error_message.txt', self.error_speak_color,
          sub_error_message = f'{min_time}から{max_time}の整数を'
        )
        print(message)
        continue
  
    return time


  def __set_all_time(self):
    """アラームを鳴らし始める時刻、アラームを止める時刻、何分ごとにアラームを鳴らすか、
       を設定し、CSVファイルに書き込む関数を呼ぶ
    """
    self.start_alarm_hour = self.__set_time(
      'アラームを鳴らし始めますか？', 'hour.txt', 0, 23
    )
    self.start_alarm_minute = self.__set_time(
      '', 'minute.txt', 0, 59
    )
    self.stop_alarm_hour = self.__set_time(
      'アラームを止めますか？', 'hour.txt', 0, 23
    )
    self.stop_alarm_minute = self.__set_time(
      '', 'minute.txt', 0, 59
    )
    self.interval_minute = self.__set_time(
      '何分おきにアラームを鳴らしますか？\n', 'minute.txt', 5, 59
    )

    self.start_alarm_time = datetime.time(self.start_alarm_hour, self.start_alarm_minute)
    self.stop_alarm_time = datetime.time(self.stop_alarm_hour, self.stop_alarm_minute)
    self.interval = datetime.timedelta(minutes=self.interval_minute)

    message = console.make_message_from_document(
      'ok_or_ng.txt', self.speak_color,
      sub_ok_or_ng_message = '以下の通りに設定しました。\n',
      sub_start_alarm_time = self.start_alarm_time,
      sub_stop_alarm_time = self.stop_alarm_time,
      sub_interval = self.interval,
    )
    print(message)

    time_data = [
      self.start_alarm_time,
      self.stop_alarm_time,
      self.interval
    ]

    if not time_data in self.history_model.history_data:
      self.history_model.history_data.append(time_data)
      self.history_model.save(self.history_model.history_data)

  
  def __use_past_settings(self):
    """履歴を使うかどうかを判別し、結果に応じてふさわしい関数を呼ぶ"""
    self.history_model.history_data = self.history_model.load_history_exclude_header()
    
    if not self.history_model.history_data:
      is_yes = False
    else:
      while True:
        message = console.make_message_from_document(
          'past_settings.txt', self.speak_color
        )
        input_is_yes = input(message)
        if input_is_yes.lower() == 'n' or input_is_yes.lower() =='no':
          is_yes = False
          break
        elif input_is_yes.lower() == 'y' or input_is_yes.lower() == 'yes':
          is_yes = True
          break
        else:
          message = console.make_message_from_document(
            'error_message.txt',
            self.error_speak_color,
            sub_error_message = 'y, yes, n, noのいずれかを'
          )
          print(message)
          continue

    if is_yes:
      self.__fetch_past_settings()
    else:
      self.__set_all_time()


  def __fetch_past_settings(self):
    """どの履歴を使用するか決定する"""
    rows = self.history_model.load_history_include_header()
    is_ok = False

    while True:
      message = console.make_message_from_document(
        'which_setting.txt', self.speak_color
      )
      user_id_choice = input(message)

      for j in range(1, len(rows)):
        if user_id_choice == rows[j][0]:
          message = console.make_message_from_document(
            'ok_or_ng.txt',
            self.speak_color,
            sub_ok_or_ng_message = '以下の通りに設定しました。\n',
            sub_start_alarm_time = rows[j][1],
            sub_stop_alarm_time = rows[j][2],
            sub_interval = rows[j][3]
          )
          print(message)

          self.start_alarm_time = dt.strptime(rows[j][1], '%H:%M:%S').time()
          self.start_alarm_time = datetime.datetime.combine(
            datetime.date.today(), self.start_alarm_time
          )
          self.start_alarm_time = self.start_alarm_time.time()

          self.stop_alarm_time = dt.strptime(rows[j][2], '%H:%M:%S').time()
          self.stop_alarm_time = datetime.datetime.combine(
            datetime.date.today(), self.stop_alarm_time
          )
          self.stop_alarm_time = self.stop_alarm_time.time()

          self.interval = dt.strptime(rows[j][3], '%H:%M:%S').time()
          self.interval = datetime.timedelta(minutes = self.interval.minute)

          is_ok = True
          break

      if is_ok:
        break
      else:
        message = console.make_message_from_document(
          'error_message.txt',
          self.error_speak_color,
          sub_error_message = '表示されているIDのいずれかを'
        )
        print(message)
        continue


  def __alarm_date_delay(
    self, start_alarm_time, start_date_delay, stop_alarm_time, stop_date_delay):
    """アラームを鳴らす日付を遅らせる
          
      Args:
        start_alarm_time, stop_alarm_time (datetime.time): アラームの時刻
        start_date_delay (int): 何日遅らせるか
          
      Returns:
        (datetime.datetime): 時刻だけでなく日付まで設定した最終的なアラームの日時
    """
    return datetime.datetime.combine(
        datetime.date.today() + datetime.timedelta(days=start_date_delay), start_alarm_time
      ), datetime.datetime.combine(
        datetime.date.today() + datetime.timedelta(days=stop_date_delay),
        stop_alarm_time
      )


  def __sound(self):
    """音を流す"""
    pygame.mixer.init()
    music = None

    try:
      import settings
      if settings.MUSIC_FILE_NAME:
        music = settings.MUSIC_FILE_NAME
    except ImportError:
      pass
    if music is None:
      music = console.find_document('sample.mp3')

    pygame.mixer.music.load(music)
    pygame.mixer.music.set_volume(DEFAULT_SOUND_VOLUME)

    now = dt.now().strftime('%H:%M')
    message = console.make_message_from_document(
      'hello.txt', self.error_speak_color, sub_now = now
    )
    print(message)
    pygame.mixer.music.play(-1)
    time.sleep(60)
    pygame.mixer.music.stop()


  def set_alarm(self):
    """アラームをセットし、鳴らす"""
    self.__use_past_settings()

    now = datetime.datetime.now().time()
    sub_now = datetime.datetime.now()
    sub_now = sub_now.replace(microsecond=0)

    """
    Now<=start<=stop
    日付変更なし

    now<=stop<startあるいはStop<now<=start
    ストップを一日ずらす

    start<=stop<nowあるいはstart<now<=stop
    スタートストップを一日ずらす

    stop<start<now
    スタートを一日、ストップを二日ずらす
    """
    if now <= self.start_alarm_time and self.start_alarm_time <= self.stop_alarm_time:
      self.start_alarm_time, self.stop_alarm_time \
        = self.__alarm_date_delay(self.start_alarm_time, 0, self.stop_alarm_time, 0)

    elif now <= self.stop_alarm_time and self.stop_alarm_time < self.start_alarm_time:
      self.start_alarm_time, self.stop_alarm_time \
        = self.__alarm_date_delay(self.start_alarm_time, 0, self.stop_alarm_time, 1)
        
    elif self.stop_alarm_time < now and now <= self.start_alarm_time:
      self.start_alarm_time, self.stop_alarm_time \
        = self.__alarm_date_delay(self.start_alarm_time, 0, self.stop_alarm_time, 1)
        
    elif self.start_alarm_time <= self.stop_alarm_time and self.stop_alarm_time < now:
      self.start_alarm_time, self.stop_alarm_time \
        = self.__alarm_date_delay(self.start_alarm_time, 1, self.stop_alarm_time, 1)
        
    elif self.start_alarm_time < now and now <= self.stop_alarm_time:
      self.start_alarm_time, self.stop_alarm_time \
        = self.__alarm_date_delay(self.start_alarm_time, 1, self.stop_alarm_time, 1)
        
    elif self.stop_alarm_time < self.start_alarm_time and self.start_alarm_time < now:
      self.start_alarm_time, self.stop_alarm_time \
        = self.__alarm_date_delay(self.start_alarm_time, 1, self.stop_alarm_time, 2)

    wait_time = self.start_alarm_time - sub_now
    time.sleep(wait_time.seconds)

    while True:
      if self.start_alarm_time <= self.stop_alarm_time:
        self.__sound()
      else:
        break
      time.sleep(self.interval.seconds - 60)
      self.start_alarm_time += self.interval