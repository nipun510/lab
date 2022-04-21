#include "util.h"

#include <iostream>
#include <thread>
#include <chrono>


class shellCommand
{
  public:
    virtual void run() = 0;
};

class printCommand : public shellCommand
{

  public:
    void run() override {
      std::cout << "hi " << std::endl;
    }
};

class helpCommand : public shellCommand
{
  public:
    void run() override {
      printCommands();
    }
  private:
    void printCommands() {
      std::cout << "help\n";
      std::cout << "print\n";
    }
};

class invalidCommand : public shellCommand
{
  public:
    void run() override {
      std::cout << "invalid command\n";
    }
};


std::shared_ptr<shellCommand> 
parseLine(const std::string &cmdStr) 
{
  auto tokens = util::split(cmdStr);
  std::string subCmdStr = tokens[0];
  if (subCmdStr == "help") {
    return std::make_shared<helpCommand>();
  } 
  else if (subCmdStr == "print" ) {
    return std::make_shared<printCommand>();
  } else {
    return std::make_shared<invalidCommand>();
  }
}

int main() 
{
  std::string line;
  std::cout << "shell>";
  while(true) {
    std::getline(std::cin, line);
    if (line.empty()) {
      std::cout << "shell>";
      continue;
    }
    std::shared_ptr<shellCommand> cmd = parseLine(line);
    cmd->run();
    std::cout << "shell>";
    
  }
#if 0
  int count = 100;
  for (int i = 0; i < count; ++i) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << i << '\r';
  }
#endif
}
