#include <iostream>
#include <string>

template<typename T>
class Interface {
  public:
  void print() {
    T &underlying = static_cast<T &>(*this);
    std::cout << underlying.getValue() << std::endl;
  }
};


class Imp1 : public Interface<Imp1>
{
  public:
  Imp1(int val): _val{val} {} 
  int getValue() const {return _val;};
  private:
    int _val;
};

class Imp2 : public Interface<Imp2>
{
  public:
  Imp2(const std::string &val) : _val{val} {}  
  std::string getValue() const {return _val;}
  private:
   std::string _val;
};

template<typename T>
void printable(Interface<T> &obj) {
  obj.print();
}


int main() {
  Imp1 i1(23);
  Imp2 i2("hello");
  i1.print();
  i2.print();
  printable(i1);
  printable(i2);
}
