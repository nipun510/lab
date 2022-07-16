
#include <unordered_map>
#include <string>


class person {
  public:
    person(unsigned id, const std::string &name, int age): _id{id}, _name{name}, _age{age} {}
    int getId() const { return _id;}
    std::string getName() const { return _name;}
    int getAge() const { return _age;}
  private:
    unsigned _id;
    std::string _name;
    int _age;
};

bool operator == (const person &lhs, const person &rhs) {
  return (lhs.getId() == rhs.getId());
}
class personHash {
  public:
    size_t operator()(const person &p) const {
      return (std::hash<std::string>()(p.getName()) ^
             std::hash<int>()(p.getAge()));  
    }
};

int main() {
  std::unordered_map<person, int, personHash> pm;
  person p1{1, "jonny", 19};
  person p2{2, "scott", 19};
  pm.insert({p1, 12});
  pm.insert({p2, 12});
}

