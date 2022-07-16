public enum LogLevel
{
    None = 0,                 //        0
    Info = 1,                 //        1
    Debug = 2,                //       10
    Warning = 4,              //      100
    Error = 8,                //     1000
    FunctionalMessage = 16,   //    10000
    FunctionalError = 32,     //   100000
    All = 63                  //   111111
}

/// <summary>
/// Abstract Handler in chain of responsibility pattern.
/// </summary>
public abstract class Logger
{
    protected LogLevel logMask;
    protected Logger next;

    public Logger(LogLevel mask)
    {
        this.logMask = mask;
    }
    public Logger SetNext(Logger nextlogger)
    {
        Logger lastLogger = this;

        while (lastLogger.next != null)
        {
            lastLogger = lastLogger.next;
        }

        lastLogger.next = nextlogger;
        return this;
    }

    public void Message(string msg, LogLevel severity)
    {
        if ((severity & logMask) != 0) // True only if any of the logMask bits are set in severity
        {
            WriteMessage(msg);
        }
        if (next != null) 
        {
            next.Message(msg, severity); 
        }
    }

    abstract protected void WriteMessage(string msg);
}

public class ConsoleLogger : Logger
{
    public ConsoleLogger(LogLevel mask)
        : base(mask)
    { }

    protected override void WriteMessage(string msg)
    {
        Console.WriteLine("Writing to console: " + msg);
    }
}

public class EmailLogger : Logger
{
    public EmailLogger(LogLevel mask)
        : base(mask)
    { }
}
class FileLogger : Logger
{
    public FileLogger(LogLevel mask)
        : base(mask)
    { }

    protected override void WriteMessage(string msg)
    {
        // Placeholder for File writing logic
        Console.WriteLine("Writing to Log File: " + msg);
    }
}

