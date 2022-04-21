#include <iostream>
#include <unordered_map>
#include <any>
#include <variant>
#include <functional>
#include <type_traits>
#include <utility>
#include <exception>

class argument_not_found : std::exception
{
  public:
    argument_not_found(const std::string &msg): _msg{msg}{}
    const char * what() const noexcept override{
      return _msg.c_str();
    }
  private:
    std::string _msg;  
};

class argParser;
class argument 
{
  public:
    argument() = default;
    argument(const std::string &name): _name{name} {}
    template<typename F>
    void action(F &&f, std::enable_if_t<std::is_invocable_v<F, std::string>, std::nullptr_t> = nullptr) {
      using actionType = std::conditional_t<std::is_void_v<std::invoke_result_t<F, std::string>>, voidAction, valuedAction>;
      _action.emplace<actionType>(f);
    }

  private:
    friend class argParser;
    using voidAction = std::function<void(const std::string &)>;
    using valuedAction = std::function<std::any(const std::string &)>;

    std::string _name;
    std::string _value;
    std::string _help;
    bool _hasValue{false};
    bool _isRequired;
    std::variant<voidAction, valuedAction> _action{std::in_place_type<valuedAction>, [&](const std::string &value) {return _value;}};
};

class argParser 
{
  public:
    argument &add_argument(const std::string &argName); 
    template<typename T> 
    T get(const std::string &argName);
    void scan(int argc, char *argv[]);
  private:
  std::unordered_map<std::string, argument> _args;
};

argument&
argParser::add_argument(const std::string &argName) 
{
  argument arg(argName);
  _args[argName] = arg;
  return _args[argName];
}

template<typename T>
T argParser::get(const std::string &argName) 
{
  auto arg = _args[argName];
  if (!arg._hasValue) {
    throw argument_not_found(argName + " not found");
  }
  auto fun = std::get<argument::valuedAction>(arg._action);
  return std::any_cast<T>(std::invoke(fun, arg._value));
}

void
argParser::scan(int argc, char *argv[]) {
  for (int i = 0; i < argc; ++i) {
    for (auto & nameValue: _args) {
      if (argv[i] == nameValue.first) {
        std::string argValue = argv[i + 1];
        nameValue.second._value = argValue;
        nameValue.second._hasValue = true;
        ++i;
      }
    }
  }
}

int main(int argc, char *argv[]) {
  argParser p;
  p.add_argument("--value1").action([](const std::string &valueStr) -> int {return std::stoi(valueStr);});
  p.add_argument("--value2").action([](const std::string &valueStr) -> float {return std::stof(valueStr);});
  p.scan(argc, argv);
  try 
  {
    int val1 = p.get<int>("--value1");
    float val2 = p.get<float>("--value2");
    std::cout << val1 << " " << val2 << std::endl;
  }
  catch(argument_not_found e) 
  {
    std::cout << e.what() << std::endl;
  }
  
}
