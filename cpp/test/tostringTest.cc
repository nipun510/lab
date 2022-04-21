#include <util/tostring.h>

#include <iostream>
#include <sstream>


class A{
  public:
    A () : _val1{3}, _val2{23.32} {}
    std::string toString() const {
      std::ostringstream os;
      os << "A : {\n";
      os << "      _val1 : " << stringify::toString(_val1) << "\n";
      os << "      _val2 : " << stringify::toString(_val2) << "\n";
      os << "    }";
      return os.str();

    }
    int _val1;
    float _val2;
};

class B{
  public:
    B () : _val2{3323.434} {}
    std::string toString() const {
      std::ostringstream os;
      os << "B : {\n";
      os << "      _val1 : " << stringify::toString(_val1) << "\n";
      os << "      _val2 : " << stringify::toString(_val2) << "\n";
      os << "    }";
      return os.str();
    }
    A _val1;
    float _val2;
};

int main() {
  int val1 = 32;
  std::cout << stringify::toString(val1) << std::endl;
  float val2 = 32.232;
  std::cout << stringify::toString(val2) << std::endl;
  double d = 232.232;
  std::cout << stringify::toString(d) << std::endl;
  char c = 'a';
  std::cout << stringify::toString(c) << std::endl;
  bool flag = false;
  std::cout << stringify::toString(flag) << std::endl;
  A obj1;
  std::cout << stringify::toString(obj1) << std::endl;
  B obj2;
  std::cout << stringify::toString(obj2) << std::endl;
  std::cout << stringify::toString(val1, val2, d, c) << std::endl;
  char a[3] = {'a', 'b', 'c'};
  std::cout << stringify::toString(a) << std::endl;
  const char* str = "hi hello";
  std::cout << stringify::toString(str) << std::endl;
  std::cout << stringify::toString("hello world") << std::endl;
}
