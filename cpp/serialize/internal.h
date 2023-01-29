#ifndef INTERNAL_H
#define INTERNAL_H

#include <iostream>
#include <concepts>
#include <memory>
#include <type_traits>

template<typename T>
struct is_sequential_container : std::false_type {};

template<typename T>
struct is_sequential_container<std::vector<T, std::allocator<T>>> : std::true_type {};


template<typename T>
inline constexpr bool is_sequential_container_v = is_sequential_container<T>::value;

template<typename T>
inline void log(const T &val) {
  std::cout << val;
}

template<typename T, typename... Args>
inline void log(const T &val, Args &&... args) {
  log(val);
  std::cout << " ";
  log(std::forward<Args>(args)...);
}


template<typename... Args>
inline void logln(Args &&... args) {
  log(std::forward<Args>(args)...);
  std::cout << std::endl;
}

#define INFO(...) \
logln(std::string(__FILE__) + ":" + std::string(__func__) \
      + ":" + std::to_string(__LINE__), ##__VA_ARGS__);

template<typename T>
char *as_bytes(const T &val) {
   void *addr = const_cast<T*>(&val);
   return static_cast<char *>(addr);
}

#endif
