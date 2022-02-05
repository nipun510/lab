package main

import (
  "fmt"
  "os"
  "golang.org/x/net/html"
)


func main() {
  doc, err := html.Parse(os.Stdin)
  if err != nil {
    fmt.Fprintf(os.Stderr, "linkExtractor %v", err)
    os.Exit(1)
  }
  for _, link := range visit(nil, doc) {
    fmt.Println(link)
  }
}

func visit(links []string, n *html.Node) []string
{
  if n.Type == html.ElementNode && n.Data == "a" {
    for _, a := range n.Attr {
      if a.key == "href" {
      }
    }
  }
}

