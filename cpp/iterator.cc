#include <vector>
#include <iostream>


template<typename T>
class IterInterface {
public:
	virtual bool _isValid() = 0;
	virtual void _doNext() = 0;
	virtual T&   _doValue() = 0;
	virtual void _dorewind() = 0;

	operator bool() {
		return _isValid();
	}

	T& operator++() {
		_doNext();
		return _doValue();
	}

	T& operator++(int) {
		_doNext();
		return _doValue();
	}

	T& operator*() {
		return _doValue();
	}
	

	T* operator->() {
		return &(_doValue());
	}
};

class Animal;
class AnimalVisitor{
public:
  bool visitAnimal(Animal &animal); 
};



class Animal {
public:
	enum class type {
		CAT,
		DOG,
		COW
	};

    void accept(AnimalVisitor &visitor) {
    	visitor.visitAnimal(*this);
    }

	Animal(const std::string &name, type t): _name{name}, _type{t} {}
	type getType() const {
		return _type;
	}
	const std::string &getName() const {
		return _name;
	}
private:
	std::string _name;
	type _type;
	
};

bool AnimalVisitor::visitAnimal(Animal &animal) {
  	std::cout << "[" << animal.getName() << "]\n";
  	return true;
}

class Zoo {
  public:
    class CatIterator : public IterInterface<Animal>{
      public:
    	using animalIterType = std::vector<Animal>::iterator;
    	CatIterator(animalIterType start, animalIterType end) : _start{start}, _curr{start}, _end{end}{
    		_dorewind();
    	}
      private:
    	bool _isValid() {
    		return _curr != _end && _curr->getType() == Animal::type::CAT;
    	}
    	void _doNext() {
    		if (_curr == _end) return;

    		++_curr;
    		while(_curr != _end) {
    			if (_curr->getType() == Animal::type::CAT) {
    				break;
    			}
    			++_curr;
    		}
    	}
    	void _dorewind(){
    		_curr = _start;
    		if (!_isValid()) {
    			_doNext();
    		}
    	}

    	Animal& _doValue(){
    		return *_curr;
    	}
    	animalIterType _start;
    	animalIterType _curr;
    	animalIterType _end;	
    };


  bool traverseAnimals(AnimalVisitor &visitor) {
  	for (auto &animal : _animals) {
  		if (!visitor.visitAnimal(animal)) {
  			return false;
  		}
  	}
  	return true;
  }

  void addAnimal(const Animal &animal) {
		_animals.push_back(animal);
	}
  CatIterator getCatIter() {
    	return CatIterator(_animals.begin(), _animals.end());
    }



    
private:
	std::vector<Animal> _animals;
};


















int main() {
	Zoo z;
	z.addAnimal(Animal{"cat1", Animal::type::CAT});
	z.addAnimal(Animal{"dog", Animal::type::DOG});
	z.addAnimal(Animal{"cat2", Animal::type::CAT});
	z.addAnimal(Animal{"dog2", Animal::type::DOG});

	for (auto it = z.getCatIter(); it; it++) {
		std::cout << it->getName() << std::endl;
	}
	AnimalVisitor v;
	z.traverseAnimals(v);
}