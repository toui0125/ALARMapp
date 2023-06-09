#pragma once


#include <string>
#include <vector>
#include <sqlite3.h>


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
  public:
    sqlite3* db_conn;
    int query_result;
    char* db_err_msg;
    DbModel(const char* db_file);
};


class HistoryModel : public DbModel
{
  public:
    HistoryModel(const char* db_file) : DbModel(db_file){};

    std::vector<HistoryColumnWithoutId> history_data;

    std::vector<HistoryColumnWithoutId> load_history_exclude_header();

    std::vector<HistoryColumn> load_history_include_header();

    void save(const std::vector<HistoryColumnWithoutId>& history_data);
};