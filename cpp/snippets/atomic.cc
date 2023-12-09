#include <vector>
#include <atomic>
#include <thread>
#include <iostream>
#include <chrono>


std::vector<int> data;
std::atomic<bool> data_ready{false};


void reader_thread()
{
  while(!data_ready.load()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  std::cout << "data is " << data[0] << std::endl;
}

void writer_thread()
{
  data.push_back(10);
  data_ready = true;
}

int main() {
  std::thread t1(reader_thread);
  std::thread t2(writer_thread);
  t1.join();
  t2.join();
}
