
#include "fileOps.h"
#include <iostream>
#include <assert.h>

int main() {
  {
    std::string s1 = "hello";
    std::string s2 = "hello";
    int d = editDistance(s1, s2);
    assert(d == 0 && "edit distance is wrong");
  }
  {
    std::string s1 = "hello";
    std::string s2 = "hei";
    int d = editDistance(s1, s2);
    assert(d == 3 && "edit distance is wrong");
  }
  {
    std::string s1 = "hello";
    std::string s2 = "cello";
    int d = editDistance(s1, s2);
    assert(d == 1 && "edit distance is wrong");
  }
  {
    std::vector<std::string> lines1 {"hello", "hi", "bye"};
    std::vector<std::string> lines2 {"hello", "bye", "welcome"};
    auto result = filesDiff(lines1, lines2);
    for (auto lineType : result) {
      std::cout << lineType.first << " " << lineType.second << std::endl;
    }
  }
}
