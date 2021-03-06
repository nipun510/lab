
#include "json.h"
#include "jsonValue.h"
#include "jsonEncoderImp.h"

#include <cassert>

json::json(): _value{nullptr}, _encoder{nullptr} {}
json::json(int val) : _value{new jsonValue{val}}, _encoder{new defaultEncoder{}} {}
json::json(bool val) : _value{new jsonValue{val}}, _encoder{new defaultEncoder{}} {}
json::json(const std::string &val) : _value{new jsonValue{val}}, _encoder{new defaultEncoder{}} {}
json::json(const char * val) : json(new jsonValue{val}), _encoder{new defaultEncoder{}}{}
json::json(const std::nullptr_t &val) : _value{new jsonValue{val}}, _encoder{new defaultEncoder{}} {}

json::~json() 
{
  if (_value) {
    delete _value;
    _value = nullptr;
  }
  if (_encoder) {
    delete _encoder;
    _encoder = nullptr;
  }
}

bool json::isValueBoolType() const { return _value->isValueBoolType(); }
bool json::isValueNullType() const { return _value->isValueNullType(); }
bool json::isValueNumberType() const { return _value->isValueNumberType(); }
bool json::isValueIntType() const { return _value->isValueIntType(); }
bool json::isValueDoubleType() const { return _value->isValueDoubleType(); }
bool json::isValueStringType() const { return _value->isValueStringType(); }
bool json::isValueArrayType() const { return _value->isValueArrayType(); }
bool json::isValueObjectType() const { return _value->isValueObjectType(); }

void 
json::setEncoder(jsonEncoder *encoder) 
{
  assert(encoder);
  if (_encoder) {
    delete _encoder;
    _encoder = nullptr;
  }
  _encoder = encoder->clone();
}


std::string
json::dumps() const
{
  return _encoder->decode(this);
}

bool
json::operator ==(const json &left, const json &right) 
{
  return (left._value == right._value);
}


bool
json::operator !=(const json &left, const json &right) 
{
  return !(left == right);
}

jason::operator bool(const json &j) 
{
  return (j._value != nullptr);
}
