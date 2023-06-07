#pragma once


#include<sqlite3.h>
#include <string>
#include <vector>


#define HISTORY_COLUMN_ID "ID"
#define HISTORY_COLUMN_START_ALARM_TIME "START_ALARM_TIME"
#define HISTORY_COLUMN_STOP_ALARM_TIME "STOP_ALARM_TIME"
#define HISTORY_COLUMN_INTERVAL "INTERVAL"


struct HistoryColumnWithoutId
{
  std::string history_column_start_alarm_time;
  std::string history_column_stop_alarm_time;
  std::string history_column_interval;
};


struct HistoryColumn
{
  std::string history_column_id;
  std::string history_column_start_alarm_time;
  std::string history_column_stop_alarm_time;
  std::string history_column_interval;
};


class DbModel
{
  protected:
    std::string db_file;
  public:
    sqlite3* db;
    char* errmsg;
    int rc;
    DbModel(std::string db_file);
};


class HistoryModel : public DbModel
{
  public:
    std::vector<HistoryColumnWithoutId> history_data;

    HistoryModel(std::string db_file = "");

    std::vector<HistoryColumnWithoutId> load_history_exclude_header();

    std::vector<HistoryColumn> load_history_include_header();

    void save(const std::vector<HistoryColumnWithoutId>& history_data);
};