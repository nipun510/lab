package main

import (
  "os"
  "fmt"
)


func main() {
  if len(os.Args)  == 1 {
    fmt.Println("No test provided")
    return
  }
  if os.Args[1] == "dcache" {
    runNode(os.Args[2])
  }
}
