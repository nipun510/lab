package main

import (
  net
  fmt
  os
)


func main() {
  conn, err := net.dial("tcp", "")
  if err != nil {
    log.Fatal("Failed to connect\n")
  }
}
