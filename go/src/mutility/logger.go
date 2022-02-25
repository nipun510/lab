package mutility

import (
  "log"
  "os"
  "io/ioutil"
)

var (
    InfoLogger    *log.Logger
    DebugLogger   *log.Logger
    WarningLogger *log.Logger
    ErrorLogger   *log.Logger
)

func init() {
    DebugLogger   = log.New(os.Stdout, "DEBUG: ", log.Ldate|log.Ltime|log.Lshortfile)
    InfoLogger    = log.New(os.Stdout, "INFO: ", log.Ldate|log.Ltime|log.Lshortfile)
    WarningLogger = log.New(os.Stdout, "WARNING: ", log.Ldate|log.Ltime|log.Lshortfile)
    ErrorLogger   = log.New(os.Stdout, "ERROR: ", log.Ldate|log.Ltime|log.Lshortfile)
    DebugLogger.SetOutput(ioutil.Discard)
}
