// visitor pattern

// go doesn't support overloading, so the visit methods need different names.
type Visitor interface {
	visitWheel(wheel Wheel) string
	visitEngine(engine Engine) string
	visitBody(body Body) string
	visitCar(car Car) string
}


// java example
interface CarElement {
    void accept(CarElementVisitor visitor);
}

interface CarElementVisitor {
    void visit(Body body);
    void visit(Car car);
    void visit(Engine engine);
    void visit(Wheel wheel);
}

class Body implements CarElement {....}
class Engine implements CarElement {....}
class Wheel implements CarElement {....}
class Car implements CarElement {....}


class CarElementPrintVisitor implements CarElementVisitor {...}
class CarElementDoVisitor implements CarElementVisitor {...}

// c++ example
class Operation;

class Object 
{
  public:
  virtual accept(const Operation &op) = 0;
};


class A : public Object 
{
  void accept(const Operation &op) override;

};
class B : public Object 
{
  void accept(const Operation &op) override;
};
class C : public Object 
{
  void accept(const Operation &op) override;

};

// .cc implementation
# include <operation.h>
void
A::accept(const Operation &op) 
{
  op.operationOnA(*this); // 2nd dispatch, call mapped to actual concrete op here
}
void
B::accept(const Operation &op) 
{
  op.operationOnB(*this);
}
void
C::accept(const Operation &op) 
{
  op.operationOnC(*this);
}
// end of .cc implementation

// visitor
class Operation 
{
  public:
    void visit(const Object &obj) { 
      obj.accept(*this); // 1st dispatch
    }
    virtual void operationOnA(const A &) = 0;
    virtual void operationOnB(const B &) = 0;
    virtual void operationOnC(const C &) = 0;
};

class OpX : public Operation
{
  public:
    void operationOnA(const A&) override;
    void operationOnB(const B&) override;
    void operationOnC(const C&) override;
};
class OpY : public Operation
{
  public:
    void operationOnA(const A&) override;
    void operationOnB(const B&) override;
    void operationOnC(const C&) override;
};
class OpZ : public Operation
{
  public:
    void operationOnA(const A&) override;
    void operationOnB(const B&) override;
    void operationOnC(const C&) override;
};

// user code
Object * obj = new B(); 
Operation *op = new OpX();
obj->accept(*op); // 1st dispatch, call mapped to actual concrete obj(B here)
//or
op->visit(obj); // here no dispatch

