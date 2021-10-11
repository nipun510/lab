#include <iostream>


template <typename T, typename Parameter>
struct namedType {
  public:
    explicit namedType(const T &val): _val(val) {}
    explicit namedType(T &&val): _val{std::move(val)}{}
    T& get() {return _val;}
    const T& get() const {return _val;}
  private:
    T _val;
};

using meter = namedType<double, struct meterParam>;
meter operator"" _meter(unsigned long long  val) {
  return meter(val);
}
using radiusInMeter = namedType<meter, struct radiusParam>;
std::ostream &operator << (std::ostream &os, const meter &val) {
  os << val.get();
  return os;
}


struct circle {
  public:
    circle(radiusInMeter val) : r(val) {}
    void printRadius() const{
      std::cout << r.get() << std::endl;
    }
  private:
    radiusInMeter r;
};


int main() {
  circle c(radiusInMeter(2_meter));
  c.printRadius();
}

