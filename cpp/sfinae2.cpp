#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

/*
 the point of SFINAE is to deactivate a piece of template code for some types.
*/

template<bool Conditional, typename T>
struct enable_if {};

template<typename T>
struct enable_if<true, T> {
    using type = T;
};

template<typename T>
using isConvertibleToInt = std::vector<T>;

template<typename T, typename = enable_if_t<std::is_convertible<T,int>::value, std::nullptr_t>>
void fun(T val, enable_if_t<std::is_convertible<T, int>::value, std::nullptr_t>) {
    std::cout << " this is int" << val << std::endl;
}

template<typename T>
void fun(T val, typename enable_if<std::is_convertible<T, std::string>::value, std::nullptr_t>::type = nullptr) {
    std::cout << " this is string" << val << std::endl;
}

int main()
{
    fun(23);
    fun("ererre");
}
