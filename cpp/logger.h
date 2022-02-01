#ifndef  LOGGER_H
#define  LOGGER_H

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

    static std::string logLevelStr(logger::level l) 
    {
      std::map<logger::level, std::string> levelStr = {
      {logger::level::DEBUG, "debug"},
      {logger::level::INFO,  "info"},
      {logger::level::WARN,  "warn"},
      {logger::level::ERROR,  "error"}};
      return levelStr[l];
    }

    static void logPrefix (logger::level l, const std::string &sourceFileName, unsigned sourceLineNo);

    template<typename T>
    static void log(const T& val);

    template<typename T, typename ...Args>
    static void log(const T& val, const Args& ...args);
  private:
};


inline std::string currTime() 
{
  char timeStr[100];
  std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::strftime(timeStr, sizeof(timeStr), "%c", std::localtime(&t));
  const std::string currTime(timeStr);
  return currTime;
}

inline void
logger::logPrefix (logger::level l, 
           const std::string &sourceFileName, 
           unsigned sourceLineNo) 
{
  std::cout <<  (std::string("[") + logger::logLevelStr(l) +  "][" + (sourceFileName + ":" + stringify::toString(sourceLineNo)) + "][" + currTime() + "] "); 
}

template<typename T>
inline void
logger::log(const T &val) 
{
  std::cout <<  stringify::toString(val)  << std::endl;
}

template<typename T, typename ...Args>
inline void
logger::log(const T &val, const Args& ...args)
{
  std::cout << stringify::toString(val); 
  std::cout << stringify::toString<Args...>(args...) << std::endl; 
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

#endif
