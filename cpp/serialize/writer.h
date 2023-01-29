#ifndef WRITER_H
#define WRITER_H

#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <type_traits>

#include "internal.h"

class simpleWriter;
template<typename T>
concept HasWriteMethod = requires(T v, simpleWriter &sw) {
  {v.write(sw)} -> std::same_as<bool>;
};

template<typename T>
concept Writable = HasWriteMethod<T> or 
                   std::is_fundamental_v<T> or 
                   std::is_same_v<T, std::string> or
                   is_sequential_container_v<T>;

class simpleWriter
{
public:
  simpleWriter(std::ofstream &of): _fs{of} {}

  simpleWriter(const simpleWriter&) = delete;
  simpleWriter & operator = (const simpleWriter &) = delete;
  simpleWriter(simpleWriter&&) = delete;
  simpleWriter & operator = (simpleWriter &&) = delete;

  template<typename T>
  requires Writable<T>
  bool write (const T &val) 
  {
    if constexpr (std::is_fundamental_v<T>) {
      //INFO("fundamental");
      _fs.write(as_bytes(val), sizeof(val));
      return isValid();

    // string
    } else if constexpr (std::is_same_v<std::string, T>){
      INFO("string");
      size_t len = val.length();
      write(len);
      for (const auto & elem: val) {
        write(elem);
      }
      return isValid();

    // sequential container
    } else if constexpr (is_sequential_container_v<T>) {
      INFO("sequential_container");
      size_t len = val.size();
      _fs.write(as_bytes(len), sizeof(len));
      for (const auto & elem : val) {
        write(elem);
      }
      return isValid();

    } else {
      INFO("writeMethod");
      return val.write(*this);
    }
  }

  bool isValid() const {
    return _fs.good();
  }

private:
  std::ofstream &_fs;
};

#endif
