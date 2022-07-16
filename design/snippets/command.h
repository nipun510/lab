

public interface Command 
{
  public void execute();
  public void undo();
}



// concrete command
public class LightOnCommand implements Command 
{
  Light light; //receiver

  public LightOnCommand(Light light) 
  {
     this.light = light;
  }
  public void execute() 
  {
     light.on();
  }
  public void undo() 
  {
   light.off();
  }
}

// concrete command
public class LightOffCommand implements Command {
  Light light; // receiver
  public LightOffCommand(Light light) {
  this.light = light;
  }
  public void execute() {
    light.off();
  }
  public void undo() {
    light.on();
  }
}

// invoker
public class RemoteControlWithUndo {
  Command[] onCommands;
  Command[] offCommands;
  Command undoCommand;

  public void onButtonWasPushed(int slot) 
  {
    onCommands[slot].execute();
    undoCommand = onCommands[slot];
  }
  public void offButtonWasPushed(int slot) {
    offCommands[slot].execute();
    undoCommand = offCommands[slot];
  }
}

