#include <util/typetrait.h>

struct A{
    using type = int;
    int a;
    void fun();
};

struct B {
};


int main()
{
    static_assert(is_void<void>::value);
    static_assert(is_nullptr<std::nullptr_t>::value);
    static_assert(is_integral<int>::value);
    static_assert(is_array<int[3]>::value);
    static_assert(is_class<A>::value);
    static_assert(is_member_pointer<double (A::*)>::value);
    static_assert(is_member_pointer<decltype(&A::fun)>::value);
    static_assert(std::is_member_function_pointer<decltype(&A::fun)>::value);
    static_assert(is_member_function_pointer<decltype(&A::fun)>::value);
    static_assert(is_member_pointer<int (A::*)>::value);
    static_assert(has_type_member<A>::value);
    static_assert(!has_type_member<B>::value);
    static_assert(!has_type_member<int>::value);
    
}
