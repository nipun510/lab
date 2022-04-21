
#ifndef JSON_TYPES_H
#define JSON_TYPES_H

#include <string>
#include <vector>
#include <map>
#include <variant>

class jsonValue;
using boolType = bool;
using nullType = std::nullptr_t;
using numberType = std::variant<int, double>;
using stringType = std::string;
using arrayType = std::vector<jsonValue*>;
using objectType = std::map<std::string, jsonValue*>;
using jsonValueType = std::variant<boolType, int, double, stringType, nullType, arrayType, objectType>;

using jsonArrayIter = arrayType::iterator;
using jsonObjectIter = objectType::iterator;
using jsonArrayConstIter = arrayType::const_iterator;
using jsonObjectConstIter = objectType::const_iterator;

#endif
