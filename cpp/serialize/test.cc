#include <iostream>
#include <fstream>
#include "readerWriter.h"

struct A
{
  bool write(writer &w) const
  {
    w.write(val1);
    w.write(val2);
    w.write(str);
    w.write(vec);
    w.write(vec2);
    return w.isValid();
  }

  bool read(reader &r)
  {
    r.read(val1);
    r.read(val2);
    r.read(str);
    r.read(vec);
    r.read(vec2);
    return r.isValid();
  }
  void print() {
   std::cout << val1 << " " << val2 << " " << str.length() << ":" << str << std::endl;
  }

  int val1;
  int val2;
  std::string str;
  std::vector<int> vec;
  std::vector<std::vector<int>> vec2;
};

template<typename T>
bool isEqual(const T&lhs, const T& rhs){ return false;}

template<>
bool isEqual<A> (const A &lhs, const A &rhs) 
{
  return lhs.val1 == rhs.val1 &&
         lhs.val2 == rhs.val2 &&
         lhs.str == rhs.str &&
         lhs.vec == rhs.vec &&
         lhs.vec2 == rhs.vec2;
}

int main() 
{
  A lhs{50, 100, std::string("hi hellow how are you"), {2,3,4}, {{2,3}, {323,43}}};
  A rhs;
  const std::string filename{"file.txt"};
  {
    std::ofstream of(filename, std::ios_base::binary);
    simpleSerializer::writer s(of);
    s.write(lhs);
  }
  {
    std::ifstream ifs(filename, std::ios_base::binary);
    simpleSerializer::reader s(ifs);
    s.read(rhs);
  }
  if(isEqual(lhs, rhs)) {
    std::cout << "validation successful\n";
  } else {
    std::cout << "validation failed\n";
    lhs.print();
    rhs.print();
  }
}
