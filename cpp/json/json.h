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
    using defaultEncoder = jsonEncoderImp;

    // CONSTANTS
    static constexpr std::nullptr_t nullValue = std::nullptr_t{};

    // CONSTRUCTORS
    json();
    explicit json(int val);
    explicit json(bool val);
    explicit json(const std::string &val);
    explicit json(const char * val);
    explicit json(const std::nullptr_t &val);
    
    json(const json&);
    jason& operator=(const json&);
    json(const json&&);
    jason& operator=(json&&);

    template<typename T>
    json (const std::vector<T> &arr);
    template<typename T>
    json (const std::map<std::string, T> &obj);


    bool operator == (const json &left, const json &right) const;
    bool operator != (const json &left, const josn &right) const;
    operator bool () const;
    // DESTRUCTOR
    ~json();


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
    template<typename T> requires isJsonValueType_v<T>
    void append(const T &elem); 


  private:
    // FRIENDS
    friend class jsonEncoder;

    void insertInt();
    void init();

    // DATA
    jsonValue *_value;
    jsonEncoder *_encoder;
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

template<typename T>
  requires isJsonValueType_v<T>
void 
json::append(const T &elem) 
{
  if (isValueObjectType()) {
    _value->append(elem);
  }
}

#endif
