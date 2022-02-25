package mutility

import (
  "net"
  "log"
  "io"
  "os"
  "fmt"
)

type Response struct {
  Location string
  Time string
}

func TestNetworkClient() {
  var port string
  if (os.Args[1] == "--port") {
    port = os.Args[2]
  } else {
    log.Fatal("--port not found\n")
  }
  address := "localhost:" + port
  conn, err := net.Dial("tcp", address)
  defer conn.Close()
  if err != nil {
    log.Fatal(err)
  }
  fmt.Printf("connected to Server at address " + address + "\n")
  copy(os.Stdout, conn)
  io.WriteString(os.Stdout, "\n")
}

func copy(dst io.Writer, src io.Reader) {
  if _, err := io.Copy(dst, src); err != nil {
    log.Fatal(err)
  }
}
