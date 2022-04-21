#include <util/util.h>

#include <iostream>
#include <string>
#include <assert.h>


int main() {
  {
    std::string s{"hello world hi "};
    auto tokens = util::split(s);
    for (const auto & token : tokens) {
      std::cout << token << std::endl;
    }
    assert(tokens.size() == 3 && "tokes count mismatch");
    assert(tokens[0] == "hello" && "token at 0 mismatch");
    assert(tokens[1] == "world" && "token at 1 mismatch");
    assert(tokens[2] == "hi" && "token at 2 mismatch");
  }
  {
    std::string s;
    auto tokens = util::split(s);
    assert(tokens.empty());
  }
  {
    std::string s{"           "};
    auto tokens = util::split(s);
    assert(tokens.empty());
  }
  {
    std::string s{"    a   b   c  "};
    auto tokens = util::split(s);
    assert(tokens.size() == 3 && "tokes count mismatch");
    assert(tokens[0] == "a" && "token at 0 mismatch");
    assert(tokens[1] == "b" && "token at 1 mismatch");
    assert(tokens[2] == "c" && "token at 2 mismatch");
  }
}
