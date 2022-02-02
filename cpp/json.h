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

class json;
class jsonValue;

class jsonEncoder 
{
  public:
    virtual json*       loads(const std::string &str) const = 0;
    virtual std::string dumps(const json *jsonObj) const = 0;

    virtual jsonEncoder * clone() = 0;
};

class jsonEncoderImp  : public jsonEncoder
{
  public:
    std::string dumps(const json *jsonObj) const override;
    json* loads(const std::string &str) const override {return nullptr;}

  private:
    void dumpUtil(const jsonValue *jsonObj, std::ostringstream &os) const;
    virtual jsonEncoder * clone() override {
      return new jsonEncoderImp();
    }
};


class jsonValue 
{
  public:
    using boolType = bool;
    using nullType = std::nullptr_t;
    using numberType = std::variant<int, double>;
    using stringType = std::string;
    using arrayType = std::vector<jsonValue*>;
    using objectType = std::map<std::string, jsonValue*>;
    using jsonValueType = std::variant<boolType, int, double, stringType, nullType, arrayType, objectType>;


    template<typename T>
    struct isValueType : std::integral_constant<bool, 
                             std::is_same_v<T, boolType> ||
                             std::is_same_v<T, nullType> ||
                             std::is_same_v<T, int> ||
                             std::is_same_v<T, double> ||
                             std::is_same_v<T, arrayType> ||
                             std::is_same_v<T, objectType>>{};
    template<typename T>
    static constexpr bool isValueType_v = isValueType<T>::value;


    using jsonArrayIter = arrayType::iterator;
    using jsonObjectIter = objectType::iterator;
    using jsonArrayConstIter = arrayType::const_iterator;
    using jsonObjectConstIter = objectType::const_iterator;

    explicit jsonValue(int val) : _value{val} {}
    explicit jsonValue(bool val) : _value{val} {}
    explicit jsonValue(const std::string &val) : _value{val} {}
    explicit jsonValue(const char * val) : jsonValue(std::string(val)){}
    explicit jsonValue(const std::nullptr_t &val) : _value{val} {}

    ~jsonValue() 
    {
      if (isValueArrayType()) {
        for (jsonValue *value : std::get<arrayType>(_value)) {
          delete value;
        }
      }
      if (isValueObjectType()) {
        for (auto &value : std::get<objectType>(_value)) {
          delete value.second;
        }
      }
    }

    template<typename T>
    jsonValue (const std::vector<T> &arr) 
    {
      _value = arrayType{};
      for (const auto & elem : arr) {
        std::get<arrayType>(_value).push_back(new jsonValue(elem));
      }
    }

    template<typename T>
    jsonValue (const std::map<std::string, T> &obj) 
    {
      _value = objectType{};
      for (const auto & elem : obj) {
        std::get<objectType>(_value).insert({elem.first, new jsonValue(elem.second)});
      }
    }

    bool isValueBoolType() const{ return std::holds_alternative<boolType>(_value); }
    bool isValueNullType() const{ return std::holds_alternative<nullType>(_value); }
    bool isValueNumberType() const { return isValueIntType() || isValueDoubleType(); }
    bool isValueStringType() const { return std::holds_alternative<stringType>(_value); }
    bool isValueArrayType() const { return std::holds_alternative<arrayType>(_value); }
    bool isValueObjectType() const { return std::holds_alternative<objectType>(_value); }
    bool isValueIntType() const { return std::holds_alternative<int>(_value); }
    bool isValueDoubleType() const { return std::holds_alternative<double>(_value); }

    jsonArrayConstIter arrayConstBegin() const{ return std::get<arrayType>(_value).cbegin(); }
    jsonArrayConstIter arrayConstEnd() const{ return std::get<arrayType>(_value).cend(); }
    jsonObjectConstIter objectConstBegin() const { return std::get<objectType>(_value).cbegin(); }
    jsonObjectConstIter objectConstEnd() const { return std::get<objectType>(_value).cend(); }
    jsonArrayIter arrayBegin() { return std::get<arrayType>(_value).begin(); }
    jsonArrayIter arrayEnd() { return std::get<arrayType>(_value).end(); }
    jsonObjectIter objectBegin() { return std::get<objectType>(_value).begin(); }
    jsonObjectIter objectEnd() { return std::get<objectType>(_value).end(); }
    std::string stringValue() const{ return std::get<stringType>(_value); }
    int intValue() const{ return std::get<int>(_value); }
    double doubleValue() const{ return std::get<double>(_value); }
    bool boolValue() const{ return std::get<boolType>(_value); }


    template<typename T>
      requires isValueType_v<T>
    void insert(const std::pair<std::string, T> &elem) 
    {
      assert(isValueObjectType());
      if (isValueObjectType()) {
        std::get<objectType>(_value).insert(std::make_pair(elem.first, new jsonValue(elem.second)));
      }
    }

  private:
    jsonValueType _value;
};



class json
{
  public:
    // TYPES
    using nullType = jsonValue::nullType;
    static constexpr std::nullptr_t nullValue = std::nullptr_t{};

    // CONSTRUCTORS
    json() = default;
    explicit json(int val) : _value{new jsonValue{val}} {}
    explicit json(bool val) : _value{new jsonValue{val}} {}
    explicit json(const std::string &val) : _value{new jsonValue{val}} {}
    explicit json(const char * val) : json(new jsonValue{val}){}
    explicit json(const std::nullptr_t &val) : _value{new jsonValue{val}} {}
    
    template<typename T>
    json (const std::vector<T> &arr) : _value{new jsonValue{arr}}{}
    template<typename T>
    json (const std::map<std::string, T> &obj) : _value{new jsonValue{obj}} {} 


    // DESTRUCTOR
    ~json() {
      delete _value;
      delete _encoder;
    }

    // MANUPULATORS
    void setEncoder(jsonEncoder *encoder) 
    {
      assert(encoder);
      if (_encoder) {
        delete _encoder;
        _encoder = nullptr;
      }
      _encoder = encoder->clone();
    }

    template<typename T>
      requires jsonValue::isValueType_v<T>
    void insert(const std::pair<std::string, T> &elem) 
    {
      _value->insert(elem);
    }

    // ACCESSORS
    const jsonValue *getValue() const; 
    std::string dumps() const;

    bool isValueBoolType() const{ return _value->isValueBoolType();}
    bool isValueNullType() const{ return _value->isValueNullType();}
    bool isValueNumberType() const{ return _value->isValueNumberType();}
    bool isValueIntType() const{ return _value->isValueIntType();}
    bool isValueDoubleType() const{ return _value->isValueDoubleType();}
    bool isValueStringType() const{ return _value->isValueStringType();}
    bool isValueArrayType() const{ return _value->isValueArrayType();}
    bool isValueObjectType() const{ return _value->isValueObjectType();}


    friend class jsonEncoder;
  private:

    // Data
    jsonValue *_value {nullptr};
    jsonEncoder *_encoder = new jsonEncoderImp();
};




// inline functions definations
std::string
inline json::dumps() const
{
  return _encoder->dumps(this);
}

inline  const jsonValue *
json::getValue() const
{
  return _value;
}

std::string
inline jsonEncoderImp::dumps(const json *jsonObj) const
{
  std::ostringstream os;
  dumpUtil(jsonObj->getValue(), os);
  return os.str();
}

inline void
jsonEncoderImp::dumpUtil(const jsonValue *value, 
                                std::ostringstream &os) const
{
  // TODO implements this with std::visit
  if(value->isValueIntType()) {
    os << std::to_string(value->intValue());
  } else if(value->isValueDoubleType()) {
    os << std::to_string(value->doubleValue());
  } else if (value->isValueArrayType()) {
    os << "[";
    const char* separator = "";
    for (auto it = value->arrayConstBegin(); it != value->arrayConstEnd(); ++it) {
      os << separator << " ";
      dumpUtil(*it, os);
      separator = ", ";
    }
    os << "]";
  } else if (value->isValueObjectType()) {
    os << "{";
    const char* separator = "";
    for (auto it = value->objectConstBegin(); it != value->objectConstEnd(); ++it) {
      os << separator << "{" << it->first << ": ";
      dumpUtil(it->second, os);
      os << "}";
      separator = ", ";
    }
    os << "}";
  } else if (value->isValueBoolType()) {
    os << (value->boolValue() == true ? "true" : "false");
  } else if (value->isValueStringType()) {
    os << value->stringValue();
  } else if (value->isValueNullType()) {
    os << "null";
  }
  else {
    assert(false);
  }
}

#endif
