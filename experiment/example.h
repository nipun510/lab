#include <iostream>

template<typename T>
class person{
  public:
  void fun();
};

template<typename T>
void person<T>::fun() {
  std::cout << "fun" << std::endl;
}
