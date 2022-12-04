#include <iostream>
#include <vector>
#include <string>
#include "connection.h"
#include "message.h"
#include "client_util.h"

// string trim functions shamelessly stolen from
// https://www.techiedelight.com/trim-string-cpp-remove-leading-trailing-spaces/

const std::string WHITESPACE = " \n\r\t\f\v";

std::string ltrim(const std::string &s) {
  size_t start = s.find_first_not_of(WHITESPACE);
  return (start == std::string::npos) ? "" : s.substr(start);
}
 
std::string rtrim(const std::string &s) {
  size_t end = s.find_last_not_of(WHITESPACE);
  return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
 
std::string trim(const std::string &s) {
  return rtrim(ltrim(s));
}

std::vector<std::string> tokenize(std::string s, std::string del = ":")
{
    std::vector<std::string> list;
    int start, end = -1*del.size();
    do {
        start = end + del.size();
        end = s.find(del, start);
        list.push_back(s.substr(start, end - start));
    } while (end != -1);
    return list;
}

void print_vector(std::vector <std::string> const &vec) {
  std::cout << "The vector elements are : " << std::endl;

  for(size_t i=0; i < vec.size(); i++)
    std::cout << vec.at(i) << std::endl;
}
