#include "typetrait.h"

#include <string>
#include <type_traits>



template<typename T,
         typename std::enable_if_t<std::is_arithmetic_v<T> && 
                                  !std::is_same_v<std::remove_cvref_t<T>, bool> &&
                                  !std::is_same_v<std::remove_cvref_t<T>, char>
                                  , int> = 0>
std::string toString(const T & val) {
  return std::to_string(val);
}

template<typename T,
         typename std::enable_if_t<std::is_same_v<std::remove_cvref_t<T>, bool>, int> = 0>
std::string toString(const T & val) {
  return val == true ? "true" : "false";
}

template<typename T,
         typename std::enable_if_t<std::is_same_v<std::remove_cvref_t<T>, char>, int> = 0>
std::string toString(const T & val) {
  std::string s;
  s += val;
  return s;
}

template<typename T,
         typename std::enable_if_t<has_toString<std::remove_cvref_t<T>>::value, int> = 0>
std::string toString(const T & val) {
  return val.toString();
}

std::string toString() 
{
  return "";
}

template<typename T, size_t N>
std::string toString(const T (&arr)[N] ) {
  std::string result;
  for (int i = 0; i < N; ++i) {
    result += toString(arr[i]);
  }
  return result;
}

std::string toString(const char* val) {
  return std::string(val);
}

template<typename T, typename ...Args>
std::string toString(const T & val, const Args& ...args)
{
  return toString(val) + " " + toString(args...);
}



