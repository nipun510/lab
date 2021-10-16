
#include "tostring.h"

#include <iostream>
#include <chrono>
#include <map>


class logger
{
  struct options {
  };
  public:
    enum class level {
      DEBUG,
      INFO,
      WARN,
      ERROR
    };
    static std::map<level, std::string> levelStr;

    static void logPrefix (logger::level l, const std::string &sourceFileName, unsigned sourceLineNo);

    template<typename T>
    static void log(const T& val);

    template<typename T, typename ...Args>
    static void log(const T& val, const Args& ...args);
  private:
};

std::map<logger::level, std::string> logger::levelStr = {
  {logger::level::DEBUG, "debug"},
  {logger::level::INFO,  "info"},
  {logger::level::WARN,  "warn"},
  {logger::level::ERROR,  "error"}};

inline std::string currTime() 
{
  char timeStr[100];
  std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::strftime(timeStr, sizeof(timeStr), "%c", std::localtime(&t));
  const std::string currTime(timeStr);
  return currTime;
}

void
logger::logPrefix (logger::level l, 
           const std::string &sourceFileName, 
           unsigned sourceLineNo) 
{
  std::cout <<  (std::string("[") + logger::levelStr[l] +  "][" + (sourceFileName + ":" + toString(sourceLineNo)) + "][" + currTime() + "] "); 
}

template<typename T>
void
logger::log(const T &val) 
{
  std::cout <<  toString(val)  << std::endl;
}

template<typename T, typename ...Args>
void
logger::log(const T &val, const Args& ...args)
{
  std::cout << toString(val); 
  std::cout << toString<Args...>(args...) << std::endl; 
}



#define INFO(...) \
  logger::logPrefix(logger::level::INFO, __FILE__, __LINE__); \
  logger::log(__VA_ARGS__);

#define WARN(...) \
  logger::logPrefix(logger::level::WARN, __FILE__, __LINE__); \
  logger::log(__VA_ARGS__);

#define ERROR(...) \
  logger::logPrefix(logger::level::ERROR, __FILE__, __LINE__); \
  logger::log(__VA_ARGS__);

