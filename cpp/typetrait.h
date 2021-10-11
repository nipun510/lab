#include <iostream>
#include <string>
#include <vector>

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


struct A{
    int a;
    void fun();
};


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

