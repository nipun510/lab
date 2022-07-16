


class Creator {
 public:
  virtual ~Creator(){};

  virtual Product* FactoryMethod() const = 0;

  std::string SomeOperation() const {
    Product* product = this->FactoryMethod();
     ...
  }
};


class ConcreteCreator1 : public Creator {
 public:
  Product* FactoryMethod() const override {
    return new ConcreteProduct1();
  }
};
class ConcreteCreator2 : public Creator {
 public:
  Product* FactoryMethod() const override {
    return new ConcreteProduct2();
  }
};



void ClientCode(const Creator& creator) {
  // ...
  std::cout << "Client: I'm not aware of the creator's class, but it still works.\n"
            << creator.SomeOperation() << std::endl;
  // ...
}
