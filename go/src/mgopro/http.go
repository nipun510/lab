package main
import (
  "fmt"
  "net/http"
  "time"
)

func hello(w http.ResponseWriter, req *http.Request) {
  fmt.Fprintf(w, time.Now().Format("11:02:23"))
}

func main() {
  http.HandleFunc("/hello", hello)
  http.ListenAndServe(":8000", nil)
}
