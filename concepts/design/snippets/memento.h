

// originator : the object for which the sate is to be saved.
class Editor {

  private:
  std::string state1;
};

 
// memento : object that is used to store the state of originator
class Memto {
  Memto(Editor *editor);
  void restore();  

  private
  std::string state1
  Editor *editor;
}

// caretaker: object  that keeps track of multiple memto, 
   like maintaing savepoints
class Command {
  
  void setMemto(Memto *memto);
  void undo();

  private:
  Memto  *memto;
}


