#include "history.h"
#include "console.h"


#include <fstream>
#include <sstream>
#include <iostream>


CsvModel::CsvModel(std::string csv_file)
{
  this->csv_file = csv_file;
  if (this->csv_file.empty()) {
    this->csv_file = "history.csv";
  }
  std::ifstream infile(this->csv_file.c_str());
  if(!infile){
    std::ofstream outfile(this->csv_file.c_str());
    outfile.close();
  }
}


HistoryModel::HistoryModel(std::string csv_file) : CsvModel(csv_file){}


std::vector<HistoryColumnWithoutId> HistoryModel::load_history_exclude_header()
{
  std::ifstream in_csv_file(csv_file);
  std::string history_data_element;
  std::string cell;
  HistoryColumnWithoutId data = {};

  std::getline(in_csv_file, history_data_element);

  while (std::getline(in_csv_file, history_data_element)) {
    std::istringstream cells(history_data_element);

    std::getline(cells, cell, ',');

    std::getline(cells, cell, ',');
    data.history_column_start_alarm_time = cell;

    std::getline(cells, cell, ',');
    data.history_column_stop_alarm_time = cell;

    std::getline(cells, cell, ',');
    data.history_column_interval = cell;

    history_data.push_back(data);
  }

  in_csv_file.close();
  return history_data;
}


std::vector<HistoryColumn> HistoryModel::load_history_include_header()
{
  std::ifstream in_csv_file(csv_file);
  std::string row;
  std::string cell;
  HistoryColumn data;

  std::vector<HistoryColumn> rows;

  while(std::getline(in_csv_file, row)){
    std::istringstream cells(row);

    std::getline(cells, cell, ',');
    data.history_column_id = cell;

    std::getline(cells, cell, ',');
    data.history_column_start_alarm_time = cell;

    std::getline(cells, cell, ',');
    data.history_column_stop_alarm_time = cell;

    std::getline(cells, cell, ',');
    data.history_column_interval = cell;

    rows.push_back(data);
  }

  std::cout << rows[0].history_column_id<<"　";
  std::cout << rows[0].history_column_start_alarm_time<<"　";
  std::cout << rows[0].history_column_stop_alarm_time<<"　";
  std::cout << rows[0].history_column_interval<<"　";
  std::cout << std::endl;

  for (int i=1; i<rows.size(); i++) {
    std::cout << rows[i].history_column_id;
    std::cout << "　　　　" << rows[i].history_column_start_alarm_time;
    std::cout << "　　　　　　" << rows[i].history_column_stop_alarm_time;
    std::cout << "　　　　　" << rows[i].history_column_interval << std::endl;
  }

  in_csv_file.close();
  return rows;
}


void HistoryModel::save(const std::vector<HistoryColumnWithoutId>& history_data)
{
  std::vector<std::string> csv_file_column = {
    HISTORY_COLUMN_ID,
    HISTORY_COLUMN_START_ALARM_TIME,
    HISTORY_COLUMN_STOP_ALARM_TIME,
    HISTORY_COLUMN_INTERVAL,
  };

  std::ofstream out_csv_file(csv_file);
  out_csv_file << csv_file_column[0] << ","
  << csv_file_column[1] << ","
  << csv_file_column[2] << ","
  << csv_file_column[3] << "\n";
  for (int id =1; id<=history_data.size(); id++) {
    auto& data = history_data[id-1];
    out_csv_file << id << ","
    << data.history_column_start_alarm_time << ","
    << data.history_column_stop_alarm_time << ","
    << data.history_column_interval << "\n";
  }
  out_csv_file.close();
}