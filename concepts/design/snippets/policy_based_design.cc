
// depend upon interface/abstraction not upon concrete classes 
template<typename T>
class messageQueue : std::queue<T>
{
  ...

  private:
    IMutex *mutex;
};


// template policy based design
template<typename T, typename Mutex_t>
class messageQueue : std::queue<T>
{
  private:
    Mutex_t mutex;
};


//usage is decided at compile time as below

#ifdef Windows
  using Mutex_t = winMutex;
#else 
  using Mutex_t = std::mutex;
#endif

  messageQueue<int, Mutex_t> msgQ;
