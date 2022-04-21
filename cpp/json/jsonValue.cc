
#include "jsonValue.h"

jsonValue::~jsonValue() 
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

