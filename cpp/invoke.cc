#include <iostream>
#include <string>
#include <functional>


template<typename T, typename Callable>
auto apply(T &val, Callable c) -> decltype(std::invoke(c, val)) {
    return std::invoke(c, val);
}

class A {
    public:
    void fun() {
        std::cout << "fun method\n";
    }
    int val{3};
};

int main()
{
    int a = 23;
    apply(a, [](int val) {std::cout << val + 1;});
    A obj;
    std::cout << apply(obj, &A::val) << std::endl;
    apply(obj, &A::fun);
}

