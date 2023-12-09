
public abstract class caffeineBeverageWithHook {
  final void prepareRecipe() 
  {
    boilWater();
    brew();
    pourInCup();
    if (customerWantCondiments()) { // hook
      addContiments();
    }  
  }
  void boilWater() {
    System.out.println("Boiling Water");
  }

  abstract void brew();

  void pourInCup() {
    System.out.println("Pouring into Cup");
  }


  boolean customerWantsCondiments() {
    return true;
  }
};

public class CoffeeWithHook extens CaffeineBeverageWithHook {

  public void brew
  {
    System.out.println("Dripping coffee through filter");
  }

  public void addCondiments() 
  {
    System.out.println("Adding sugar and Milk")
  }
};

public class TeaWithHook extens CaffeineBeverageWithHook {

  public void brew
  {
    System.out.println("Steeping the tea");
  }

  public void addCondiments() 
  {
    System.out.println("Adding lemon")
  }
};
