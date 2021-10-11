#include <type_traits>
#include <tuple>
#include <iostream>

struct realClass {};

struct proxyClass 
{
    public:
    using proxyTag = void;

};


template<typename T, typename R = void>
struct enable_if
{
    using type = R;
};

template<typename T, class Enable = void>
struct isProxy : std::false_type
{
};

template<typename T>
struct isProxy<T, typename enable_if<typename T::proxyTag>::type> : std::true_type
{};

int main() 
{
    static_assert(isProxy<realClass>::value);
}