package main

import (
  "fmt"
)

func main() {
  arr := [...]int{4,5,5,4,6,92}
  s1 := arr[1:4]
  s2 := arr[:]
  for  _, val := range s1 {
    fmt.Println("%d", val)
  }
}
