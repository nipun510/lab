#include <iostream>

class singleton {
  public:
  static singleton& getInstance() {
    static singleton instance; // lazily evaluated and thread safe
    return instance;
  }

  int getData() const {
    return _data;
  }

  // delete copy and move constructors and assignments
  singleton(const singleton &) = delete;
  singleton(singleton &&) = delete;
  singleton& operator=(const singleton &) = delete;
  singleton& operator=(singleton &&) = delete;

  ~singleton() = default;

  private:
  singleton() : _data{23} {}

  private:
  int _data;
};


int main() {
  singleton &s = singleton::getInstance();
  std::cout << "data [" << s.getData() << "]\n";
}


/*
Note: variable initialization is thread safe in c++11.   

For implementation before c++11.
Approch:

singleton* singleton::instance() {
  Lock l;
  if (_instance == nullptr) {
    _instance = new singleton();
  }
  return _instance;
}

Issue: Lock is acquired in each access of singleton _instance, however it is needed 
just for thread safety in _instance creation.


Approch: Double Checked locking pattern
singleton *singleton::instance() {
  if (_instance == nullptr) {
    Lock l;
    if (_instance == nullptr) { // to allow just the first thread to procced and disallow subseqent onces which passed the 1st null check.
      _instance = new singleton();
    }
  }
  return _instance;
}

Issue: instruction ordering in the assigment may cause an ill formed singleton object to be returned in certain scenario.
instructions:
  - allocate memory
  - construct a singleton object
  - make _instance point to the allocated memory.
  If due to instruction ordering, step1 and step3 are executed and that thread is suspended, then subsequent thread will return ill
  formed singleton object.

Conclusion:
  Avoid implementing singleton with DCLP. Better cache _instance return to avoid repeated access in the 1st approach.

*/ 
