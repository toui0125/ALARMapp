#pragma once


#include <string>
#include <exception>


std::string get_document_dir_path();

std::string find_document(const std::string& document_file_name);

class NoDocumentError : public std::exception{
  private:
    std::string err_msg;
  public:
    NoDocumentError(std::string err_msg);

    const char* what();
};

std::string get_document(
  const std::string& document_file_name, const std::string& color
);

std::string make_message_from_document(
  const std::string& document_file_name,
  const std::string& color,
  const std::vector<std::string>& replace
);