#pragma once


#include <string>


std::string get_document_dir_path();

std::string find_document(const std::string& document_file_name);

std::string get_document(
  const std::string& document_file_name, const std::string& color
);

std::string make_message_from_document(
  const std::string& document_file_name,
  const std::string& color,
  const std::vector<std::string>& replace
);