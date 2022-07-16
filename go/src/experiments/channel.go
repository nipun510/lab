package main

import (
  "fmt"
  "time"
)

func response() (string) {
  return "hello"
}

func number(in chan<- int) {
  for i:= 1; i < 100; i++ {
    in <- i
  }
  close (in)
}

func square(out <-chan int , in chan<- int) {
    for x := range out {
      in <- (x*x)
    }
    close(in)
}

func printer(out <-chan int) {
  for val := range out {
    fmt.Printf("%d ", val)
  }
}

func rocketIgnition(in chan<- int) {
  time.Sleep(3 *time.Second)
  in <- 3
}

func main() {
  //pipeline
  n := make(chan int)
  s := make(chan int)
  go number(n)
  go square(n, s)
  printer(s)


  // fastest response
  b := make(chan string, 3)
  go func() { b <- response()} ()
  go func() { b <- response()} ()
  go func() { b <- response()} ()
  val := <- b
  fmt.Println(val)


  // rocket launch

  abort := make(chan int, 1)
  go rocketIgnition(abort)

  for countdown := 10 ; countdown >= 0 ; countdown-- {
    fmt.Printf("count down %d\n", countdown)
    select {
      case <- abort : {
        fmt.Println("aborted")
        return
      }
      default : {
        time.Sleep(1 *time.Second)
      }
    }
  }

}
