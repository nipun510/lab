#include <iostream>
#include <vector>
#include <optional>
#include <map>
#include <filesystem>

#include "logger.h"

class change;

class revision
{
public:
  unsigned getId() const { return _id;}
  static unsigned generateNextId();

private:
    unsigned _id;
};

bool 
operator == (const revision &lhs, const revision &rhs)
{
  return lhs.getId() == rhs.getId();
}  

bool 
operator != (const revision &lhs, const revision &rhs)
{
  return !(lhs == rhs);
}  

class file 
{
public:
  file(const file&) = delete;
  file& operator= (const file&) = delete;

  void applyChange (const change &c); 
  revision getRevision() const;

private:
    std::vector<std::vector<char>> _rows;
    std::string _dir;
    std::string _name;
    revision   _revision;
};

class change {
  public:
  revision getFileRevision() const;
  private:
  file _file;
};

class delChange : change
{
  private:
    file _file;
};

class addChange : change
{
  private:
    file _file;
};

class editChange : change
{
  public:
    enum class changeType{
      add,
      edit,
      del
    };
  changeType getChangeType() const { return _type;}  
  private:
    changeType _type;
    unsigned   _lineNo;
    file       _file;
    std::optional<std::string> _newValue;
};

using changeList = std::vector<change>;

revision
change::getFileRevision() const
{
  return _file.getRevision();
}

unsigned
revision::generateNextId() 
{
  static unsigned id = 0;
  return id++;
}

revision
file::getRevision() const
{

  return _revision;
}

void
file::applyChange(const change & c)
{
  if (c.getFileRevision() != _revision) {
    throw std::runtime_error("invalid operation\n");
  }
}



class command
{
  public:
  virtual void execute() = 0;
  private:
};


class statCommand : public command
{
  public:
  statCommand(const std::string &filePath) : _filePath{filePath} {}
  void execute() {
    std::string msg = "stat " + _filePath + "\n";
    INFO(msg);
  } 
  std::string _filePath;
};


class initCommand : public command
{
  public:
    initCommand(const std::string &dir) : _dirPathStr{dir} {}
    void execute() override;
  private:
    std::string _dirPathStr;
};


void
initCommand::execute() 
{
  INFO("Initializing dir\n");
  std::filesystem::path dirPath(_dirPathStr);
  if (std::filesystem::exists(dirPath) ) {
    std::runtime_error("init dir already exists\n");
    return;
  }
  std::filesystem::create_directory(_dirPathStr);
  auto repoDirPath = dirPath / ".repo"; 
  std::filesystem::create_directory(repoDirPath);
}


bool
parseCmdArgs(int argc, char* argv[]) 
{
  std::vector<std::string> args;
  for (int i = 1; i < argc; ++i) {
    args.push_back(argv[i]);
  }
  if (args.empty()) {
    ERROR("invalid arguments\n");
    return false;
  }
  std::string cmdStr = args[0];
  if (cmdStr == "stat") {
    if (args.size() != 2) {
      ERROR("invalid arguments\n");
      return false;
    }
    statCommand cmd(args[1]);
    cmd.execute();
  } else if (cmdStr == "init") {
   if (args.size() != 2) {
     ERROR("invalid arguments\n");
      return false;
    }
    const std::string dirPathStr = args[1];
    initCommand cmd(dirPathStr);
    cmd.execute();
    return true;
  }
  return false;

}

int main(int argc, char *argv[])
{
  parseCmdArgs(argc, argv);
  return 0;
}

