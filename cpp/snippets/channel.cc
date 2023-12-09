#include <condition_variable>
#include <thread>
#include <chrono>
#include <iostream>


template<typename T>
class channel
{
  public:
    channel() = default;
    T receive();
    void send(T &&val);
    channel(const channel&) = delete;
    channel & operator=(const channel&) = delete;

  private:
  T _data;
  std::atomic_bool _dataAvailable{false};
  std::condition_variable _cv;
  std::mutex _mutex;
};

template<typename T>
T channel<T>::receive() 
{
    std::unique_lock<std::mutex> lk(_mutex);
    _cv.wait(lk, [&]{return _dataAvailable == true;});
    return _data;
}

template<typename T>
void channel<T>::send(T &&val) 
{
  {
    std::unique_lock<std::mutex> lk(_mutex);
    _data = val; 
    _dataAvailable = true;
  }
  _cv.notify_all();
}


class select
{
  public:
  class selectCase{
    public:
    selectCase(int id, const std::function<void()> &fn) : _id{id}, _fn{fn}{}
    void addDefault(const std::function<void()> defaultFn);

  }

  private:
  std::vector<selectCase> _cases;
}



int main() {
  channel<int> c;
  std::thread t([&c]{ 
  std::this_thread::sleep_for(std::chrono::seconds(2));
  c.send(23);});
  int val = c.receive();
  std::cout << "Value received is " << val << std::endl;
  t.join();
  return 0;
}
