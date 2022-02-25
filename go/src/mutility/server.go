package mutility

import (
  "log"
  "flag"
  "os"
  "io"
  "net"
  "time"
  "encoding/json"
)


type response struct {
  Location string
  Time string
}

// service to return current time based on location
func handleTimeRequest(c net.Conn, location string) {
  defer c.Close()
  timeloc, err := time.LoadLocation(location)
  if err != nil {
    log.Fatal("location not valid\n")
  }
  timeStr := time.Now().In(timeloc).Format("2006-01-02 15:04:05")
  res := &response{Location : location, Time    : timeStr} 
  // only those members are marshalled which starts with Caps.
  jsonResponse, err := json.Marshal(res)
  if (err != nil) {
    log.Fatal("Failed to marshal response\n")
  }
  if _, err := io.WriteString(c, string(jsonResponse)); err != nil {
    return
  }
}


func startTimeService(port, location string) {
  address := "localhost:" + port
  listener, err := net.Listen("tcp", address)
  InfoLogger.Println("listening to port " + address)

  if err != nil {
    log.Fatal(err)
  }
  // handle requests
  for {
    conn, err := listener.Accept()
    if err != nil {
      log.Print(err)
      continue
    }
    log.Print("accepted a connection\n")
    go handleTimeRequest(conn, location)
  }
}


func printInfo() {
  println("available services are: 'time'")
  cmd := os.Args[0]
  println("sample commands")
  println(cmd + " time --port 8000 --location US/Eastern")
}

func TestServer() {

  helpCommand := flag.NewFlagSet("help", flag.ExitOnError)

  startTimeServiceCommand := flag.NewFlagSet("time", flag.ExitOnError)
  port := startTimeServiceCommand.String("port", "8000", "port no for server")
  location := startTimeServiceCommand.String("location", "", "timezone for getting time")
 
  if len(os.Args) < 2 {
    log.Println("time subcommand is required")
    os.Exit(1)
  }

  switch os.Args[1] {
    case "time" : 
      startTimeServiceCommand.Parse(os.Args[2:])
    case "help" :
      helpCommand.Parse(os.Args[2:])

    default: 
      flag.PrintDefaults()
      os.Exit(1)
  }
  // starts time echo service
  if startTimeServiceCommand.Parsed() {
    startTimeService(*port, *location)
  } else if helpCommand.Parsed() {
    printInfo()
  }
}

