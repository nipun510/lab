#ifndef JSON_H
#define JSON_H

#include "jsonValue.h"
#include "jsonTypes.h"
#include "jsonTypeTraits.h"
#include "jsonEncoder.h"
#include "jsonEncoderImp.h"

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <concepts>

class json;
class jsonValue;

class json
{
  public:
    // TYPES
    using nullType = nullType;
    static constexpr std::nullptr_t nullValue = std::nullptr_t{};

    // CONSTRUCTORS
    json() = default;
    explicit json(int val);
    explicit json(bool val);
    explicit json(const std::string &val);
    explicit json(const char * val);
    explicit json(const std::nullptr_t &val);
    
    template<typename T>
    json (const std::vector<T> &arr);
    template<typename T>
    json (const std::map<std::string, T> &obj);


    // DESTRUCTOR
    ~json();

    // FRIENDS
    friend class jsonEncoder;

    // ACCESSORS
    std::string      dumps() const;
    const jsonValue* getValue() const { return _value;} 

    bool isValueBoolType() const;
    bool isValueNullType() const;
    bool isValueNumberType() const;
    bool isValueIntType() const;
    bool isValueDoubleType() const;
    bool isValueStringType() const;
    bool isValueArrayType() const;
    bool isValueObjectType() const;

    // MANIPULATORS
    void setEncoder(jsonEncoder *encoder); 

    template<typename T> requires isJsonValueType_v<T>
    void insert(const std::pair<std::string, T> &elem); 


  private:
    void insertInt();
    // Data
    jsonValue *_value = nullptr;
    jsonEncoder *_encoder{new jsonEncoderImp{}};
};

// inline functions definations

template<typename T>
json::json (const std::vector<T> &arr) : _value{new jsonValue{arr}}{}

template<typename T>
json::json (const std::map<std::string, T> &obj) : _value{new jsonValue{obj}} {} 

template<typename T>
  requires isJsonValueType_v<T>
void 
json::insert(const std::pair<std::string, T> &elem) 
{
  if (isValueObjectType()) {
    _value->insert(elem);
  }
}

#endif
