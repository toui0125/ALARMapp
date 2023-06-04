#pragma once


#include <string>
#include <vector>


#include "history.h"


#define DEFAULT_SOUND_VOLUME 1.0
#define DEFAULT_SPEAK_COLOR "\033[36m" //シアン
#define DEFAULT_ERROR_SPEAK_COLOR "\033[35m" //マゼンタ


class Robot
{
protected:
  std::string speak_color;
  std::string error_speak_color;
public:
  Robot(std::string speak_color = DEFAULT_SPEAK_COLOR,
  std::string error_speak_color = DEFAULT_ERROR_SPEAK_COLOR);
};


class AlarmRobot : public Robot
{
  private:
    std::string input;
    std::string message = "";
    std::vector<std::string> replace_word = {};
    struct tm start_alarm_time;
    struct tm stop_alarm_time;
    int interval;
    HistoryModel* history_model;

    int set_time(std::string time_message, std::string time_file,
    int min_time, int max_time);

    void set_all_time();

    void use_past_settings();

    void fetch_past_settings();

    bool isLeapYear(int year);

    struct tm alarm_date_delay(struct tm alarm_time, int date_delay);

    void sound();
  public:
    AlarmRobot(std::string speak_color = DEFAULT_SPEAK_COLOR,
    std::string error_speak_color = DEFAULT_ERROR_SPEAK_COLOR);

    ~AlarmRobot();

    void set_alarm();
};