#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <string>
#include <vector>


namespace util {

std::vector<std::string> 
split(const std::string &str, char separator = ' ') 
{
  if (str.empty()) {
    return {};
  }

  std::size_t startPos = str.find_first_not_of(separator, 0);
  if (startPos == std::string::npos) {
    return {};
  }
  std::vector<std::string> result;
  while(startPos < str.length()) {
    auto pos = str.find_first_of(separator, startPos);
    if (pos != std::string::npos) { 
      result.push_back(str.substr(startPos, pos - startPos));
      startPos = str.find_first_not_of(separator, pos + 1);
    } else {
      result.push_back(str.substr(startPos));
      break;
    }
  }

  return result;
}


class formatter 
{
  public:
    formatter() : _level{0} {}
    void print(const std::string &s, std::ostream &os);
    void incrementLevel() { ++_level; }
    void decrementLevel() { --_level; }
  private:
    size_t _level;
};




void 
formatter::print(const std::string &s, std::ostream &os = std::cout)
{
  std::string indent(_level, ' ');
  size_t start = 0;
  while (start < s.size()) {
   size_t pos = s.find('\n');
   if (pos != std::string::npos) {
     std::string line = s.substr(start, pos - start + 1);
     os << indent << line; 
     start = pos + 1;
   }
  }
}

} //namespace util

#endif
