
// builder helps avoid this
class Pizza {
    Pizza(int size) { ... }
    Pizza(int size, boolean cheese) { ... }
    Pizza(int size, boolean cheese, boolean pepperoni) { ... }
    

class Director{
    public:

    void setBuilder(Builder* builder){
        this->builder=builder;
    }

    /**
     * The Director can construct several product variations using the same
     * building steps.
     */
    void BuildMinimalViableProduct(){
        this->builder->producePartA();
    }
    
    void BuildFullFeaturedProduct(){
        this->builder->producePartA();
        this->builder->producePartB();
        this->builder->producePartC();
    }

    private:
    Builder* builder;
};    


class Builder{
    public:
    virtual ~Builder(){}
    virtual void producePartA() const =0;
    virtual void producePartB() const =0;
    virtual void producePartC() const =0;
};

// Differen builders produces different types and representation of objects.
class concreteBuilder1 : public Builder {
  void producePartA() override const;
  void producePartB() override const;
  void producePartC() override const;
  product1 * getProduct();
}
class concreteBuilder2 : public Builder {
  void producePartA() override const;
  void producePartB() override const;
  void producePartC() override const;
  product2 * getProduct();
}

int clientCode() 
{
    ConcreteBuilder1* builder = new ConcreteBuilder1();
    director.setBuilder(builder);
    director.buildMinimalViableProduct();
    product1* p= builder->getProduct();
}


// another example
#include <string>

class Automobile {
private:
    int numberOfTires;
    std::string bodyType;
    std::string engineType;
    int fuelTankSizeInGallons;
    bool airConditioned;
    int odometerMiles;

public:

    class Builder {
    public:        
        int numTires = 0;
        std::string body = "";
        std::string engine = "";
        int fuelTankSize = 0;
        bool airCondition = false;
        int odometer = 0;

        Builder* numberOfTires(int numberOfTires) {
            this->numTires = numberOfTires;
            return this;
        }

        Builder* bodyType(std::string bodyType) {
            this->body = bodyType;
            return this;
        }

        Builder* engineType(std::string engineType) {
            this->engine = engineType;
            return this;
        }

        Builder* fuelTankSizeInGallons(int fuelTankSizeInGallons) {
            this->fuelTankSize = fuelTankSizeInGallons;
            return this;
        }

        Builder* airConditioned(bool airConditioned) {
            this->airCondition = airConditioned;
            return this;
        }

        Builder* odometerMiles(int odometerMiles) {
            this->odometer = odometerMiles;
            return this;
        }

        Automobile build() {
            return Automobile(*this);
        }
    };

    Automobile(Builder builder)
            : numberOfTires(builder.numTires), bodyType(builder.body), engineType(builder.engine),
            fuelTankSizeInGallons(builder.fuelTankSize), airConditioned(builder.airCondition),
            odometerMiles(builder.odometer) {}
};

int main() {
    Automobile automobile = Automobile::Builder().numberOfTires(4)
        ->bodyType("coupe")
        ->engineType("V8")
        ->fuelTankSizeInGallons(16)
        ->airConditioned(true)
        ->odometerMiles(20000)->build();
}
