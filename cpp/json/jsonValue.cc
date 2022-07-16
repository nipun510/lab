
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

bool 
jsonValue::operator== (const jsonValue &val1, const jsonValue &val2) 
{
  if (val1

}

