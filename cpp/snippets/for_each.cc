#include <iostream>
#include <algorithm>
#include <vector>
#include <type_traits>
#include <concepts>



template<std::forward_iterator It, typename F>
  requires std::invocable<F, typename std::iterator_traits<It>::value_type>
void mfor_each(It begin, It end,  F&& f) {
  using value_type = typename std::iterator_traits<It>::value_type;
   for (auto it = begin; it != end; ++it) {
     if constexpr (std::is_same_v<std::invoke_result_t<F, value_type>, bool>) {
       if (!f(*it)) {
         break;
       }
     }
   }
}


int main() {
  std::vector<int> v{32,23,22,4,33,55,22,23};
  mfor_each(v.begin(), v.end(), [](auto val) { 
      std::cout << val  << " "; 
      return (val == 55 ? false : true);
  });
  std::cout << std::endl;
}

