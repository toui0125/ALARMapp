#include "alarm.h"
#include "console.h"


#include <iostream>
#include <vector>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <thread>


Robot::Robot(std::string speak_color, std::string error_speak_color)
{
  this->speak_color = speak_color;
  this->error_speak_color = error_speak_color; 
}


AlarmRobot::AlarmRobot(std::string speak_color, std::string error_speak_color)
:Robot(speak_color, error_speak_color)
{
  time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  start_alarm_time = *localtime(&now);
  stop_alarm_time = *localtime(&now);
  history_model = new HistoryModel();
}


AlarmRobot::~AlarmRobot()
{
  delete history_model;
}


int AlarmRobot::set_time
(std::string time_message, std::string time_file, int min_time, int max_time)
{
  int time;

  while(1){
    replace_word = {time_message, std::to_string(min_time), std::to_string(max_time)};
    message = make_message_from_document(time_file, speak_color, replace_word);
    std::cout << message << std::endl;

    std::getline(std::cin, input);
    try {
      time = std::stoi(input);
    } catch (std::invalid_argument&) {
      replace_word = {
        std::to_string(min_time) + "から" + std::to_string(max_time) + "までの整数を"
      };
      message = make_message_from_document(
        "error_message.txt", error_speak_color, replace_word
      );
      std::cout << message << std::endl;
      continue;
    }
    if (min_time <= time && time <= max_time) {
      break;
    } else {
      replace_word = {
        std::to_string(min_time) + "から" + std::to_string(max_time) + "までの整数を"
      };
      message = make_message_from_document(
        "error_message.txt", error_speak_color, replace_word
      );
      std::cout << message << std::endl;
      continue;
    }
  }

  return time;
}


void AlarmRobot::set_all_time()
{
  int start_alarm_hour = set_time("アラームを鳴らし始めますか？", "hour.txt", 0, 23);
  int start_alarm_minute = set_time("", "minute.txt", 0, 59);
  int stop_alarm_hour = set_time("アラームを止めますか？", "hour.txt", 0, 23);
  int stop_alarm_minute = set_time("", "minute.txt", 0, 59);
  interval = set_time("何分おきにアラームを鳴らしますか？\n", "minute.txt", 1, 59);

  start_alarm_time.tm_hour = start_alarm_hour;
  start_alarm_time.tm_min = start_alarm_minute;
  start_alarm_time.tm_sec = 0;

  stop_alarm_time.tm_hour = stop_alarm_hour;
  stop_alarm_time.tm_min = stop_alarm_minute;
  stop_alarm_time.tm_sec = 0;

  std::ostringstream start_alarm_time_to_string;
  start_alarm_time_to_string << std::setw(2) << std::setfill('0')
  << start_alarm_hour << ":" << std::setw(2) << std::setfill('0') << start_alarm_minute;
  std::string string_start_alarm_time = start_alarm_time_to_string.str();

  std::ostringstream stop_alarm_time_to_string;
  stop_alarm_time_to_string << std::setw(2) << std::setfill('0')
  << stop_alarm_hour << ":" << std::setw(2) << std::setfill('0') << stop_alarm_minute;
  std::string string_stop_alarm_time = stop_alarm_time_to_string.str();

  std::ostringstream interval_to_string;
  interval_to_string << std::setw(3) << std::setfill('0')
  << ":" << std::setw(2) << std::setfill('0') << interval;
  std::string string_interval = interval_to_string.str();

  replace_word = {
    "以下の通りに設定しました。\n",
    string_start_alarm_time,
    string_stop_alarm_time,
    string_interval
  };
  message = make_message_from_document(
    "ok_or_ng.txt", speak_color, replace_word
  );
  std::cout << message << std::endl;

  HistoryColumnWithoutId time_data = {	
    string_start_alarm_time, string_stop_alarm_time, string_interval
  };
  bool found = false;
  for (auto& data : (history_model -> history_data)) {
    if (data.history_column_start_alarm_time == time_data.history_column_start_alarm_time
    && data.history_column_stop_alarm_time == time_data.history_column_stop_alarm_time
    && data.history_column_interval == time_data.history_column_interval) {
      found = true;
      break;
    }
  }
  if (!found) {
    (history_model -> history_data).push_back(time_data);
    history_model -> save(history_model -> history_data);
  }
}


void AlarmRobot::use_past_settings()
{
  history_model -> history_data = history_model -> load_history_exclude_header();
  bool is_yes;

  if ((history_model -> history_data).empty()) {
    is_yes = false;
  } else {
    while (1) {
      message = get_document("past_settings.txt", speak_color);
      std::cout << message << std::endl;

      std::getline(std::cin, input);
      std::transform(input.begin(), input.end(), input.begin(), ::tolower);
      if (input == "n" || input == "no") {
        is_yes = false;
        break;
      } else if(input == "y" || input == "yes") {
        is_yes = true;
        break;
      } else {
        replace_word = {"y, yes, n, noのいずれかを"};
        message = make_message_from_document(
          "error_message.txt", error_speak_color, replace_word
        );
        std::cout << message << std::endl;
        continue;
        }
    } 
  }
  if (is_yes) {
    fetch_past_settings();
  } else {
    set_all_time();
  }
}


void AlarmRobot::fetch_past_settings()
{
  std::vector<HistoryColumn> rows = history_model -> load_history_include_header();
  bool is_ok = false;

  while(1){
    message = get_document("which_setting.txt", speak_color);
    std::cout<<message<<std::endl;

    std::getline(std::cin, input);
    for(int i=1; i<rows.size(); i++){
      if(input == rows[i].history_column_id){
        replace_word = {
          "以下の通りに設定しました。\n",
          rows[i].history_column_start_alarm_time,
          rows[i].history_column_stop_alarm_time,
          rows[i].history_column_interval
        };
        message = make_message_from_document("ok_or_ng.txt", speak_color, replace_word);
        std::cout << message << std::endl;

        std::size_t found;
        
        found = rows[i].history_column_start_alarm_time.find(":");
        if (found != std::string::npos) {
          std::string start_alarm_hour
          = rows[i].history_column_start_alarm_time.substr(0, found);
          std::string start_alarm_min
          = rows[i].history_column_start_alarm_time.substr(found + 1);

          start_alarm_time.tm_hour = std::stoi(start_alarm_hour);
          start_alarm_time.tm_min = std::stoi(start_alarm_min);
          start_alarm_time.tm_sec = 0;
        }

        found = rows[i].history_column_stop_alarm_time.find(":");
        if (found != std::string::npos) {
          std::string stop_alarm_hour
          = rows[i].history_column_stop_alarm_time.substr(0, found);
          std::string stop_alarm_min
          = rows[i].history_column_stop_alarm_time.substr(found + 1);

          stop_alarm_time.tm_hour = std::stoi(stop_alarm_hour);
          stop_alarm_time.tm_min = std::stoi(stop_alarm_min);
          stop_alarm_time.tm_sec = 0;
        }

        found = rows[i].history_column_interval.find(":");
        if (found != std::string::npos) {
          std::string s_interval = rows[i].history_column_interval.substr(found + 1);

          interval = std::stoi(s_interval);
        }

        is_ok = true;
        break;
      }
    }
    if (is_ok) {
      break;
    } else {
      replace_word = {"表示されているIDのいずれかを"};
      message = make_message_from_document(
        "error_message.txt", error_speak_color, replace_word
      );
      std::cout << message << std::endl;
      continue;
    }
  }
}


bool AlarmRobot::isLeapYear(int year)
{
  if (year % 4 == 0) {
    if (year % 100 == 0) {
      if (year % 400 == 0) {
        return true;
      }
      return false;
    }
    return true;
  }
  return false;
}


struct tm AlarmRobot::alarm_date_delay(struct tm alarm_time, int date_delay)
{
  alarm_time.tm_mday += date_delay;
  int days_in_month;

  switch (alarm_time.tm_mon + 1) {
    case 2:
      if (isLeapYear(alarm_time.tm_year + 1990)) {
        days_in_month = 29;
      } else {
        days_in_month = 28;
      }
      break;
    case 4:
    case 6:
    case 9:
    case 11:
      days_in_month = 30;
      break;
    default:
      days_in_month = 31;
      break;
  }
  if (alarm_time.tm_mday > days_in_month) {
    alarm_time.tm_mday = 1;
    alarm_time.tm_mon += 1;
    if (alarm_time.tm_mon > 11) {
      alarm_time.tm_mon = 1;
      alarm_time.tm_year += 1;
    }
  }

  return alarm_time;
}


void AlarmRobot::sound()
{
  time_t now
  = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  struct tm sub_now = *localtime(&now);

  std::ostringstream now_to_string;
  now_to_string << std::setw(2) << std::setfill('0')
  << sub_now.tm_hour << ":" << std::setw(2) << std::setfill('0') << sub_now.tm_min;
  std::string string_now = now_to_string.str();

  replace_word = {string_now};
  message = make_message_from_document("hello.txt", error_speak_color, replace_word);
  std::cout << message << std::endl;
}


void AlarmRobot::set_alarm()
{
  use_past_settings();
  time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

  if (difftime(mktime(&stop_alarm_time), mktime(&start_alarm_time)) >= 0
  && difftime(mktime(&start_alarm_time), now) >= 0) {
    start_alarm_time = alarm_date_delay(start_alarm_time, 0);
    stop_alarm_time = alarm_date_delay(stop_alarm_time, 0);
  } else if (difftime(mktime(&start_alarm_time), mktime(&stop_alarm_time)) > 0
  && difftime(mktime(&stop_alarm_time), now) >= 0) {
    start_alarm_time = alarm_date_delay(start_alarm_time, 0);
    stop_alarm_time = alarm_date_delay(stop_alarm_time, 1);
  } else if (difftime(mktime(&start_alarm_time), now) >= 0
  && difftime(now, mktime(&stop_alarm_time)) > 0) {
    start_alarm_time = alarm_date_delay(start_alarm_time, 0);
    stop_alarm_time = alarm_date_delay(stop_alarm_time, 1);
  } else if (difftime(now, mktime(&stop_alarm_time)) > 0
  && difftime(mktime(&stop_alarm_time), mktime(&start_alarm_time)) >= 0) {
    start_alarm_time = alarm_date_delay(start_alarm_time, 1);
    stop_alarm_time = alarm_date_delay(stop_alarm_time, 1);
  } else if (difftime(mktime(&stop_alarm_time), now) >= 0
  && difftime(now, mktime(&start_alarm_time)) > 0) {
    start_alarm_time = alarm_date_delay(start_alarm_time, 1);
    stop_alarm_time = alarm_date_delay(stop_alarm_time, 1);
  } else if (difftime(now, mktime(&start_alarm_time)) > 0
  && difftime(mktime(&start_alarm_time), mktime(&stop_alarm_time)) > 0) {
    start_alarm_time = alarm_date_delay(start_alarm_time, 1);
    stop_alarm_time = alarm_date_delay(stop_alarm_time, 2);
  }

  double diff_seconds = std::difftime(std::mktime(&start_alarm_time), now);
  std::this_thread::sleep_for(std::chrono::seconds(static_cast<int>(diff_seconds)));

  while (1) {
    if (difftime(mktime(&stop_alarm_time), mktime(&start_alarm_time)) >= 0) {
      sound();
    } else {
      break;
    }
    std::this_thread::sleep_for(std::chrono::seconds(interval * 60));
    start_alarm_time.tm_min += interval;
  }
}