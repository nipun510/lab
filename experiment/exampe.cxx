module;

#include <iostream>

export module example

export int fun() {
  std::cout << "exported fun \n";
}


int notFun() {
  std::cout << "not exported notFun \n";
}
