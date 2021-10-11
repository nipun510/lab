#include <iostream>
#include <string>
#include <functional>
#include <vector>
#include <tuple>



template<typename F, typename... Args>
struct curry {
   // std::tuple<Args...> tuple;
   // F func;
   curry(F func, Args... args) : func(func), args(std::tuple<Args...>(args)...) {
   }
   
   curry(F func, std::tuple<Args...>(args)...) : func(func), args(std::move(args)) {}
   
   template<typename... Params> 
   auto operator()(Params... params){
       auto tuples = std::tuple_cat(args, std::tuple<Params...>(params)...);
       if constexpr (std::is_invocable_v<F, Args..., Params...>) {
           std::apply(func, tuples);
       } else {
           return curry<F, Args..., Params...>(func, tuples);
       }
   } 
   
   
   private:
   F func;
   std::tuple<Args...> args;
};


void fun(int x) {
    std::cout << "fun " << x << std::endl;
}

void fun3(int x, int y, int z) {
    std::cout << "fun " << x << " " << y << " " << z << std::endl;
}

int main()
{
   curry c(&fun);
   c(7);

   curry c2(&fun3);
   auto c3 = c2(23);
   auto c4 = c3(34);
   c4(12);
}
