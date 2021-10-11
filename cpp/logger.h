
#include <iostream>
#include <chrono>


class logger
{
  public:
    enum class level {
      DEBUG,
      INFO,
      WARN,
      ERROR
    };
    static std::map<level, std::string> levelStr;

    static void log(level l, const std::string &sourceFileName, unsigned sourceLineNo, const std::string &msg);
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
logger::log(logger::level l, const std::string &sourceFileName, unsigned sourceLineNo, const std::string &msg) 
{
  std::cout << "[" << logger::levelStr[l] <<  "][" << sourceFileName << ":" << sourceLineNo << "][" << currTime() << "] " << msg;
  
}

#define INFO(msg) \
  logger::log(logger::level::INFO, __FILE__, __LINE__, msg); 

#define WARN(msg) \
  logger::log(logger::level::WARN, __FILE__, __LINE__, msg); 

#define ERROR(msg) \
  logger::log(logger::level::ERROR, __FILE__, __LINE__, msg); 

