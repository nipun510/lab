#ifndef TYPETRAIT_H
#define TYPETRAIT_H

#include <iostream>
#include <string>
#include <vector>




// some dummy type traits implementations
template<typename T>
struct is_void : std::is_same<void, std::remove_cv_t<T>> {};

template<typename T>
struct is_nullptr : std::is_same<std::nullptr_t, std::remove_cv_t<T>> {};

template<typename T>
struct is_integral : std::integral_constant<bool, std::is_same_v<int, std::remove_cv_t<T>> || 
                                                  std::is_same_v<unsigned, std::remove_cv_t<T>> ||
                                                  std::is_same_v<char, std::remove_cv_t<T>> ||
                                                  std::is_same_v<long long, std::remove_cv_t<T>>> {};

template<typename T>
struct is_array : std::false_type {};

template<typename T>
struct is_array<T[] > : std::true_type {};

template<typename T, size_t N>
struct is_array<T[N] > : std::true_type {};

namespace detail {
    template<typename T>
    std::integral_constant<bool, !std::is_union_v<T>> test(int T::*);
    std::false_type test(...);
}

template<typename T>
struct is_class : decltype(detail::test<T>(nullptr)) {};

template<typename T>
struct is_member_pointer_helper : std::false_type {};
template<typename T, typename U>
struct is_member_pointer_helper<T U::*> : std::true_type {};
template<typename T>
struct is_member_pointer : is_member_pointer_helper<std::remove_cv_t<T>> {};

template<typename T>
struct is_member_function_pointer_helper : std::false_type {};
template<typename T, typename U>
struct is_member_function_pointer_helper<T U::*> :  std::is_function<T>{};
template<typename T>
struct is_member_function_pointer : is_member_function_pointer_helper<std::remove_cv_t<T>> {};

template<typename, typename = void>
struct has_type_member : std::false_type{};
template<typename T>
struct has_type_member<T, std::void_t<typename T::type>> : std::true_type {};

// sfinae based method detection on a type
// concept can be used c++20 onward.
template<typename T>
class has_toString 
{
  using one = char;
  using two = struct { char arr[2];};
  
  template<typename C> static one test(decltype(&C::toString));
  template<typename C> static two test(...);

  public:
  static constexpr int value = (sizeof(test<T>(0)) == 1);
};


// tag can be defined in cases, and can be used to 
// identify classes based on tag at compile time
struct realClass {};
struct proxyClass 
{
  using proxyTag = void;
};

template<typename T, class Enable = void>
struct isProxy : std::false_type {};

template<typename T>
struct isProxy<T, typename T::proxyTag> : std::true_type {};

#endif
