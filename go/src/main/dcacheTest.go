package main

import (
	"fmt"

	"lab.com/dcache"
)

func runNode(currNode string) {

	addr := "127.0.0.1"
	ports := [...]string{"6379", "6380", "6381"}

	switch currNode {
	case "node0":
		{
			fmt.Println("starting server0...")
			server0 := dcache.NewServer(addr, ports[0], "7379", "node0")
			//server0.AddMember("node1", "127.0.0.1", "6380", "7380")
			//server0.AddMember("node2", "127.0.0.1", "6381", "7381")
			server0.Start()
			fmt.Println("started server0...")
		}
	case "node1":
		{
			server1 := dcache.NewServer(addr, ports[1], "7380", "node1")
			//server1.AddMember("node0", "127.0.0.1", "6379", "7379")
			//server1.AddMember("node2", "127.0.0.1", "6381", "7381")
			server1.Start()
			fmt.Println("server1 started")
		}
	case "node2":
		{
			server2 := dcache.NewServer("127.0.0.1", ports[2], "7381", "node2")
			//server2.AddMember("node0", "127.0.0.1", "6379", "7379")
			//server2.AddMember("node1", "127.0.0.1", "6380", "7380")
			server2.Start()
			fmt.Println("server2 started")
		}
	default:
		{
			fmt.Println("unknown node")
		}
	}
}
