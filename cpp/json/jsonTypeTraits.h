
#ifndef JSON_TRAITS_H
#define JSON_TRAITS_H

#include <type_traits>

template<typename T>
struct isJsonValueType : std::integral_constant<bool, 
                         std::is_same_v<T, boolType> ||
                         std::is_same_v<T, nullType> ||
                         std::is_same_v<T, int> ||
                         std::is_same_v<T, double> ||
                         std::is_same_v<T, arrayType> ||
                         std::is_same_v<T, objectType>>{};
template<typename T>
static constexpr bool isJsonValueType_v = isJsonValueType<T>::value;

#endif
