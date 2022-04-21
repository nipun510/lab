
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>

#include "jsonEncoderImp.h"
#include "json.h"
#include "jsonValue.h"


std::string
jsonEncoderImp::decode(const json *jsonObj) const
{
  std::ostringstream os;
  decodeInt(jsonObj->getValue(), os);
  return os.str();
}

void
jsonEncoderImp::decodeInt(const jsonValue *value, 
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
    for (auto it = value->arrayValueContBegin(); it != value->arrayValueConstEnd(); ++it) {
      os << separator << " ";
      decodeInt(*it, os);
      separator = ", ";
    }
    os << "]";
  } else if (value->isValueObjectType()) {
    os << "{";
    const char* separator = "";
    for (auto it = value->objectValueConstBegin(); it != value->objectValueConstEnd(); ++it) {
      os << separator << "{" << it->first << ": ";
      decodeInt(it->second, os);
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
