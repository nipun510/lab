
#include <iostream>
#include <queue>
#include <thread>
#include <chrono>

class syncedStream
{
public:
  syncedStream(std::ostream &stream) : os(stream) {}
  template<typename Args...>
  print(const Args... args) {
    std::lock_guard<mutex> lg(streamMutex);
    os << args...;
  }

  template<typename Args...>
  println(const Args... args) {
    std::lock_guard<mutex> lg(streamMutex);
    os << args... << "\n";
  }

private:
  std::ostream &os;
  std::mutex streamMutex;
};


class taskQueue
{
public:
  taskQueue(): ss(std::cout) {}
  void submitTask();
  void runTask(); 
private:
  std::mutex coutMutext;
  std::mutex qMutex;
  std::syncedStream ss;
  std::queue<std::this_thread::id> _taskQueue;
};


taskQueue::submitTask() 
{
  std::this_thread::id id = std::this_thread::get_id();
  {
    std::lock_guard<mutex> lg(coutMutex);
    ss.println("task submitted by thread with ", id); 
  }
  std::lock_guard<mutex> lg(qMutex);
  _taskQueue.push_back(std::this_thread::id);
}

taskQueue::runTask() {
  std::this_thread::sleep_for(2s);
  {

  }

}
