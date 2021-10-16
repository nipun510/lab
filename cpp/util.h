
#include <iostream>
#include <string>

class Formatter 
{
  public:
    static void printBegin(const std::string &s, std::ostream &os);
    static void print(const std::string &s, std::ostream &os);
    static void format() { ++level; }
  private:
    static size_t level;
};

size_t Formatter::level = 0;

void 
Formatter::print(const std::string &s, std::ostream &os = std::cout)
{
  std::string indent(level, " ");
  size_t start = 0;
  while (start < s.size()) {
   size_t pos = std::find(s.begin(), s.end(), '\n');
   if (pos != std::string::npos) {
     std::string line = s.substr(start, pos - start + 1);
     os << indent << line; 
     start = pos + 1;
   }
  }
}

void
Formatter::printBegin(const std::string &s, std::ostream &os = std::cout)
{
  level = 0;
  print(s, os);
}
