#include <iostream>
#include <type_traits>


template<typename T>
class A {
  public:
  void fun(const &T val) {
    //std::cout << std::is_lvalue_reference<T>::value << std::endl;
    //std::cout << std::is_const<T>::value << std::endl;
  }
};


int main() {
  A<int> a;
  a.fun(5);

}
