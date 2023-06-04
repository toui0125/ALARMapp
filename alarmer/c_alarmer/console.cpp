#include "console.h"


#include <iostream>
#include <fstream>
#include <cstdarg>
#include <filesystem>
#include <vector>


std::string get_document_dir_path()
{
  std::filesystem::path sub_current_dir_path = std::filesystem::current_path();
  std::string current_dir_path = sub_current_dir_path.string();

  char slash = '/';
  size_t pos = current_dir_path.rfind(slash);
  if (pos != std::string::npos) {
    current_dir_path.erase(pos);
  }

  std::string document_dir_path = current_dir_path + "/c_documents/";

  return document_dir_path;
}


std::string find_document(const std::string& document_file_name)
{
  std::string document_dir_path = get_document_dir_path();
  std::string document_file_path = document_dir_path + document_file_name;

  return document_file_path;
}


std::string get_document(const std::string& document_file_name, const std::string& color)
{
  std::string document = find_document(document_file_name);

  std::ifstream document_file(document);
  if (!document_file) {
    std::cout << "cannot find " << document << std::endl;
    exit(1);
  };
  std::string content((std::istreambuf_iterator<char>(document_file)), std::istreambuf_iterator<char>());
  std::string message(content);
  std::string splitter(60, '=');
  message = color + splitter + "\n" + message + "\n" + splitter + "\033[0m";

  return message;
}


std::string make_message_from_document(
  const std::string& document_file_name,
  const std::string& color,
  const std::vector<std::string>& replace_word
)
{
  std::string message = get_document(document_file_name, color);
  for (const auto& word : replace_word) {
    message.replace(message.find("$replace"), 8, word);
  };

  return message;
}
