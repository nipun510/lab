package main

import (
  "dcache"
  "fmt"
)

func runNode(currNode string) {

  addr := "127.0.0.1"
  ports := [...]string{"6379", "6380", "6381"}

  switch currNode {
    case "node0": {
      server0 := dcache.NewServer(addr, ports[0], "node0")
      fmt.Println("server0 started")
      server0.CompleteRing([]string{"node1", "node2"})
      server0.Start()
    } 
    case "node1": {
      server1 := dcache.NewServer(addr, ports[1], "node1")
      server1.CompleteRing([]string{"node0", "node2"})
      server1.Start()
      fmt.Println("server1 started")
    }
    case "node2": {
      server2 := dcache.NewServer("127.0.0.1", ports[2], "node2")
      server2.CompleteRing([]string{"node0", "noder1"})
      server2.Start()
      fmt.Println("server2 started")
    }
    default : {
      fmt.Println("unknown node")
    }
  }
}
