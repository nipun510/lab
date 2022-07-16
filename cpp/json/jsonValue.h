
#ifndef JSON_VALUE_H
#define JSON_VALUE_H

#include "jsonTypes.h"
#include "jsonTypeTraits.h"

#include <variant>

class jsonValue 
{
  public:
    // CONSTRUCTORS
    explicit jsonValue(int val) : _value{val} {}
    explicit jsonValue(bool val) : _value{val} {}
    explicit jsonValue(const std::string &val) : _value{val} {}
    explicit jsonValue(const char * val) : jsonValue(std::string(val)){}
    explicit jsonValue(const std::nullptr_t &val) : _value{val} {}

    template<typename T>
    jsonValue (const std::vector<T> &arr); 

    template<typename T>
    jsonValue (const std::map<std::string, T> &obj); 

    bool operator== (const jsonValue &val1, const jsonValue &val2);
    // DESTRUCTOR
    ~jsonValue(); 


    // ACCESSORS
    bool isValueBoolType() const{ return std::holds_alternative<boolType>(_value); }
    bool isValueNullType() const{ return std::holds_alternative<nullType>(_value); }
    bool isValueNumberType() const { return isValueIntType() || isValueDoubleType(); }
    bool isValueStringType() const { return std::holds_alternative<stringType>(_value); }
    bool isValueArrayType() const { return std::holds_alternative<arrayType>(_value); }
    bool isValueObjectType() const { return std::holds_alternative<objectType>(_value); }
    bool isValueIntType() const { return std::holds_alternative<int>(_value); }
    bool isValueDoubleType() const { return std::holds_alternative<double>(_value); }

    std::string   stringValue() const{ return std::get<stringType>(_value); }
    int           intValue() const{ return std::get<int>(_value); }
    double        doubleValue() const{ return std::get<double>(_value); }
    bool          boolValue() const{ return std::get<boolType>(_value); }

    // ITERATORS
    jsonArrayConstIter    arrayValueContBegin() const{ return std::get<arrayType>(_value).cbegin(); }
    jsonArrayConstIter    arrayValueConstEnd() const{ return std::get<arrayType>(_value).cend(); }
    jsonObjectConstIter   objectValueConstBegin() const { return std::get<objectType>(_value).cbegin(); }
    jsonObjectConstIter   objectValueConstEnd() const { return std::get<objectType>(_value).cend(); }
    jsonArrayIter         arrayBegin() { return std::get<arrayType>(_value).begin(); }
    jsonArrayIter         arrayEnd() { return std::get<arrayType>(_value).end(); }
    jsonObjectIter        objectBegin() { return std::get<objectType>(_value).begin(); }
    jsonObjectIter        objectEnd() { return std::get<objectType>(_value).end(); }


    // MANIPULATORS
    template<typename T> requires isJsonValueType_v<T>
    void insert(const std::pair<std::string, T> &elem); 

    template<typename T> requires isJsonValueType_v<T>
    void append(const T &elem); 

  private:
    jsonValueType _value;
};

template<typename T>
jsonValue::jsonValue (const std::vector<T> &arr) 
{
  _value = arrayType{};
  for (const auto & elem : arr) {
    std::get<arrayType>(_value).push_back(new jsonValue(elem));
  }
}

template<typename T>
jsonValue::jsonValue (const std::map<std::string, T> &obj) 
{
  _value = objectType{};
  for (const auto & elem : obj) {
    std::get<objectType>(_value).insert({elem.first, new jsonValue(elem.second)});
  }
}

template<typename T>
  requires isJsonValueType_v<T>
void 
jsonValue::insert(const std::pair<std::string, T> &elem) 
{
  if (isValueObjectType()) {
    std::get<objectType>(_value).insert(std::make_pair(elem.first, new jsonValue(elem.second)));
  }
}

template<typename T>
  requires isJsonValueType_v<T>
void 
jsonValue::append(const T &elem) 
{
  if (isValueArrayType()) {
    std::get<arrayType>(_value).push_back(new jsonValue(elem));
  }
}

#endif
