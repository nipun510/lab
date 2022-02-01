#ifndef JSON_H
#define JSON_H

#include "logger.h"

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <variant>
#include <sstream>
#include <type_traits>
#include <concepts>
#include <cassert>

class json{
  public:
    using boolType = bool;
    using nullType = std::nullptr_t;
    using numberType = std::variant<int, double>;
    using stringType = std::string;
    using arrayType = std::vector<json*>;
    using objectType = std::map<std::string, json*>;
    template<typename T>
    struct isValueType : std::integral_constant<bool, 
                             std::is_same_v<T, boolType> ||
                             std::is_same_v<T, nullType> ||
                             std::is_same_v<T, int> ||
                             std::is_same_v<T, double> ||
                             std::is_same_v<T, arrayType> ||
                             std::is_same_v<T, objectType>>{};
    template<typename T>
    static inline constexpr bool isValueType_v = isValueType<T>::value;


    static constexpr std::nullptr_t nullValue = std::nullptr_t{};
    std::string dump() const;

    json() = default;
    explicit json(int val) : _value{val} {}
    explicit json(bool val) : _value{val} {}
    explicit json(const std::string &val) : _value{val} {}
    explicit json(const char * val) : json(std::string(val)){}
    explicit json(const std::nullptr_t &val) : _value{val} {}

    template<typename T>
    json (const std::vector<T> &arr) 
    {
      _value = arrayType{};
      for (const auto & elem : arr) {
        std::get<arrayType>(_value).push_back(new json(elem));
      }
    }
    

    template<typename T>
    json (const std::map<std::string, T> &obj) 
    {
      _value = objectType{};
      for (const auto & elem : obj) {
        std::get<objectType>(_value).insert({elem.first, new json(elem.second)});
      }
    }

    template<typename T>
      requires isValueType_v<T>
    void insert(const std::pair<std::string, T> &elem) 
    {
      objectType::iterator it;
      if (isValueObjectType()) {
        std::get<objectType>(_value).insert(std::make_pair(elem.first, new json(elem.second)));
      }
    }


    bool isValueBoolType() const{
      return std::holds_alternative<boolType>(_value);
    }
    bool isValueNullType() const{
      return std::holds_alternative<nullType>(_value);
    }
    bool isValueNumberType() const
    {
      return isValueIntType() || isValueDoubleType();
    }

    bool isValueStringType() const
    {
      return std::holds_alternative<stringType>(_value);
    }

    bool isValueArrayType() const
    {
      return std::holds_alternative<arrayType>(_value);
    }

    bool isValueObjectType() const
    {
      return std::holds_alternative<objectType>(_value);
    }

    bool isValueIntType() const
    {
        return std::holds_alternative<int>(_value);
    }

    bool isValueDoubleType() const
    {
        return std::holds_alternative<double>(_value);
    }

  private:
    void dumpUtil(std::ostringstream &os) const;

    // Data
    std::variant<boolType, int, double, stringType, nullType, arrayType, objectType> _value;
};




// inline functions definations
std::string
json::dump() const
{
  std::ostringstream os;
  dumpUtil(os);
  return os.str();
}

void
json::dumpUtil(std::ostringstream &os) const
{
  if(isValueIntType()) {
    os << std::to_string(std::get<int>(_value));
  } else if(isValueDoubleType()) {
    os << std::to_string(std::get<double>(_value));
  } else if (isValueArrayType()) {
    os << "[";
    const char* separator = "";
    for (const auto & elem : std::get<arrayType>(_value)) {
      os << separator << " ";
      elem->dumpUtil(os);
      separator = ", ";
    }
    os << "]";
  } else if (isValueObjectType()) {
    os << "{";
    const char* separator = "";
    for (const auto & elem : std::get<objectType>(_value)) {
      os << separator << "{" << elem.first << " : ";
      elem.second->dumpUtil(os);
      os << "}";
      separator = ", ";
    }
    os << "}";
  } else if (isValueBoolType()) {
    os << (std::get<bool>(_value) == true ? "true" : "false");
  }  else if (isValueStringType()) {
      os << std::get<std::string>(_value);
  } else if (isValueNullType()) {
    os << "null";
  }
  else {
    assert(false);
  }
}

#endif
