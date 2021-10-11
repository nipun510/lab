#include <iostream>
#include <thread>
#include <chrono>

int main() 
{
#if 0
  std::string line;
  std::cout << ">";
  while(std::getline(std::cin, line)) {
    std::cout << line << "\n";
    std::cout << ">";
    
  }
#endif
  int count = 100;
  for (int i = 0; i < count; ++i) {
    std::this_thread::sleep_for(100ms);
    std::cout << i << '\r';
  }
}
