#ifndef READER_H
#define READER_H

#include <iostream>
#include <string>
#include <fstream>
#include <type_traits>
#include <vector>

#include "internal.h"

class simpleReader;
template<typename T>
concept HasReadMethod = requires(T v, simpleReader &sr) {
  {v.read(sr)} -> std::same_as<bool>;
};

template<typename T>
concept Readable = HasReadMethod<T> or 
                   std::is_fundamental_v<T> or 
                   std::is_same_v<T, std::string> or
                   is_sequential_container_v<T>;

// Should have reference semantics
// non copyable and non movable
class simpleReader
{
public:
  simpleReader(std::ifstream &ifs): _fs{ifs}{}

  simpleReader(const simpleReader&) = delete;
  simpleReader & operator = (const simpleReader &) = delete;
  simpleReader(simpleReader&&) = delete;
  simpleReader & operator = (simpleReader &&) = delete;

  template <typename T> requires Readable<T>
  bool read(T &val) 
  {
    using value_type = std::remove_cvref_t<T>;

    if constexpr(std::is_fundamental_v<value_type>) {
      INFO("fundamental");
      _fs.read(as_bytes(val), sizeof(val));
      return isValid();

    // string
    } else if constexpr (std::is_same_v<std::string, value_type>) {
      // schema {length, {char}}
      INFO("string");
      size_t len = 0;
      read(len);
      for (size_t i = 0; i < len; ++i) {
        char c;
        read(c);
        val.push_back(c);
      }
      return isValid();

    // sequential container
    } else if constexpr (is_sequential_container_v<T>) {
      // schema {length, {value_type}}
      INFO("sequential_container");
      size_t len = 0;
      read(len);
      for (size_t i = 0; i < len; ++i) {
        typename T::value_type elem;
        read(elem);
        val.push_back(std::move(elem));
      }
      return isValid();
    } else {
      INFO("readMethod");
      return val.read(*this);
    }
  }

  bool isValid() const {
    return _fs.good();
  }

private:
  std::ifstream &_fs;
};

#endif
