package mutility

import (
	"fmt"
	"time"
)


func fib(x int) int{
  if x < 2 {
    return x
  }
  return fib(x-1) + fib(x-2)
}

func spinner(delay time.Duration) {
  for {
    for _, r := range `-\|/` {
      fmt.Printf("\r%c", r)
      time.Sleep(delay)
    }
  }
}

func runGoroutineTest() {
  const n = 45
  go spinner(100*time.Millisecond)
  fibN := fib(n)
  fmt.Printf("\rFibonacci(%d) = %d", n, fibN)
}
