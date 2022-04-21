#include <iostream>
#include <optional>
#include <atomic>
#include <mutex>


template<typename T, std::size_t N>
class ring
{
  public:  
  bool push(T &&t)
  {
    return do_push(std::move(t));
  } 

  bool push(const T &t)
  {
    return do_push(t);
  }

   std::optional<T> pop()
  {
    std::lock_guard<std::mutex> lock(_m);
    auto val = std::optional<T>();
    if (_size > 0) {
      val = _buffer[_reader];
      _reader = (_reader + 1) % N;
      _size.fetch_sub(1);
    }
    return val;
  }


  private:
  bool do_push(auto &&t) 
  {
    std::lock_guard<std::mutex> lock(_m);
    if (_size >= N) {
      return false;
    }
    _buffer[_writer] = t;
    _writer = (_writer + 1) % N;
    _size.fetch_add(1);
    return true;
  }

  std::size_t _reader{0};
  std::size_t _writer{0};
  std::array<T, N> _buffer;
  std::atomic<std::size_t> _size;
  std::mutex _m;
};


int main() {
  ring<int, 4> r;
  r.push(23);
  r.push(323);
  auto val1 = r.pop();
  auto val2 = r.pop();
  if (val1) {
      std::cout << *val1 << " ";
  }
  if (val2) {
      std::cout << *val2 << " ";
  }

}
