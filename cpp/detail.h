#include <tuple>


namespace detail
{
  template<typename T, typename Tuple>
  struct tupleIndex{
    static constexpr std::size_t value = 0;
  };

  template<typename T, typename... Args>
  struct tupleIndex<T, std::tuple<T, Args...>>
  {
    static constexpr std::size_t value = 0;
  };

  template<typename T, typename U, typename... Args>
  struct tupleIndex<T, std::tuple<U, Args...>> 
  {
    static constexpr std::size_t value = 1 + tupleIndex<T, std::tuple<Args...>>::value;
  };
};
