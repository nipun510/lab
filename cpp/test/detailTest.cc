#include <util/detail.h>

#include <tuple>

int main() {
  using tt1 = std::tuple<int, double, float>;
  static_assert(detail::tupleIndex<int, std::tuple<int, double, float>>::value == 0);
  static_assert(detail::tupleIndex<double, std::tuple<int, double, float>>::value == 1);
  static_assert(detail::tupleIndex<float, std::tuple<int, double, float>>::value == 2);
}
