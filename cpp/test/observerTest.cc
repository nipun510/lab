
#include <util/observer.h>

#include <iostream>


class observedClass1
{
  public:
    observedClass1(notification &nl) : n{&nl}{}
    void setA(int val) {
      subject *sub = n->getSubject();
      sub->notify<observedClass1>(subject::event::premodify, this, fieldEnumClass1::A);
      a = val; 
      sub->notify<observedClass1>(subject::event::postmodify, this, fieldEnumClass1::A);
    }
    void setB(int val) {
      b = val;
    }
  private:
    int a;
    int b;
    notification *n;
};


class observedClass2
{
  public:
    observedClass2(notification &nl) : n{&nl}{}
    void setC(int val) {
      c = val;
    }
    void addC1(observedClass1 &obj) {
      _objs.push_back(obj);
    }

  private:
    int c;
    std::vector<observedClass1> _objs;
    notification *n;
};


class myObserver1 : public observer
{
  public:
  myObserver1() {
    enable<observedClass1>(subject::event::premodify);
    enable<observedClass1>(subject::event::postmodify);
  }
  void preModify(const observedClass1 *objPtr, of1 field) override
  {
    std::cout << "observer[myObserver1] prefModify [observedClass1] " 
              << " field[" << static_cast<int>(field) << "]" << std::endl;
  }
  void postModify(const observedClass1 *objPtr, of1 field) override
  {
    std::cout << "observer[myObserver1] postModify [observedClass1] " 
              << " field[" << static_cast<int>(field) << "]" << std::endl;
  }
};


int main()
{
  myObserver1 obs;
  notification n;
  subject *subject = n.getSubject();
  observedClass1 obj1(n);
  observedClass1 obj2(n);
  subject->subscribe(&obs);


  obj1.setA(23);
}
