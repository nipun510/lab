package main
import (
  "fmt"
  "io"
  "log"
  "net"
  "time"
  "encoding/json"
)

func handleConn(c net.Conn) {
  defer c.Close()
  for {
    //timeNow := time.Now().Format("15:04:03")
    //j, _ := json.Marshal(timeNow)
    //_, err := io.WriteString(c, "Hello Word")
    n, err := c.fd.Write("Hello World")
    if err != nil {
      return
    }
    time.Sleep(1*time.Second)
  }
}

func main() {
  listener, err := net.Listen("tcp", "localhost:8000")
  fmt.Print("listening to port 8000\n")

  if err != nil {
    log.Fatal(err)
  }
  for {
    conn, err := listener.Accept()
    fmt.Print("accepted a connection\n")
    if err != nil {
      log.Print(err)
      continue
    }
    fmt.Print("connection accepted succesfully\n")
    handleConn(conn)
  }
}
