#include <iostream>

class any {
  public:
    template<typename T>
    explicit any(T &&value): ptr{new T{std::forward<T>(value)}},
                             _getType{[]() -> std::type_info const & {return typeid(T);},
                             _clone{[](void *otherData) -> void *{return T(*static_cast<T*>(otherData));} { }
  private:
    void *ptr;
    std::type_info const &(*_getType)();
    void* (*clone)(void *otherData);
};


template<typename T>
T& any_cast(any &obj) {
  return *static_cast<T*>(obj.ptr);
}

int main() {
  any a = 23;
  std::cout << any_cast<int>(a);

}
