
// minimize context dependency
template<typename C>
void algo(C& c)
{
    std::sort(begin(c), end(c)); //good, necessary and useful dependency
    helper(c); // bad, potentially surprising dependency
    TT var = 32; // which TT?
}



// bad
template<typename T>
class Foo {
public:
    enum { v1, v2 };
};

// good
// place non-dependent class template members 
// in a non-templated base class
struct Foo_base {
    enum { v1, v2 };
};

template<typename T>
class Foo : public Foo_base {
public:
};


// bad
template<typename T, typename A = std::allocator<T>>
class List {
public:
    struct Link {   // does not depend on template parameter A
        T elem;
        Link* pre;
        Link* suc;
    };
    using iterator = Link*;
private:
    Link* head;
};


// good
// do not over-parameterize members
struct Link {
    T elem;
    Link* pre;
    Link* suc;
};

template<typename T, typename A = std::allocator<T>>
class List2 {
public:
    using iterator = Link<T>*;
private:
    Link<T>* head;
};


//good
// use template aliases to simplify notation and hide implementation details.
template<typename T, size_t N>
class Matrix {
    // ...
    using Iterator = typename std::vector<T>::iterator;
    // ...
};




//good
// use function templates to deduce class template argument types
tuple<int, string, double> t1 = {1, "Hamlet", 3.14};   // explicit type
auto t2 = make_tuple(1, "Ophelia"s, 3.14);         // better; deduced type
