#include "json.h"
#include "logger.h"
#include <cassert>


int main() {
  {
    json j(23);
    std::string str = j.dump();
    assert(str == "23");
  }
  {
    std::string s{"hello"};
    json j(s);
    std::string str = j.dump();
    assert(str == "hello");
  }
  {
    bool val = true;
    json j(val);
    std::string  str = j.dump();
    assert(str == "true");
  }
  {
    bool val = false;
    json j(val);
    std::string  str = j.dump();
    assert(str == "false");
  }
  {
    json::nullType val = json::nullValue;
    json j(val);
    std::string str = j.dump();
    assert(str == "null");
  }
  {
    std::vector<int> arr1{23,23,4,545,4};
    std::vector<int> arr2{2,3,4,45,4};
    std::map<std::string, std::vector<int>> obj{{"name1", arr1}, {"name2", arr2}};
    json j = obj;
    j.insert(std::make_pair(std::string{"name3"}, 43));
    std::string str = j.dump();
    assert(str == "{{name1 : [ 23,  23,  4,  545,  4]}, {name2 : [ 2,  3,  4,  45,  4]}, {name3 : 43}}");
  }
}
