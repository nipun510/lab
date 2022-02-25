package main

import (
//  "sort"
  "dcache"
)

func main() {
  caches := map[string]*dcache.KVStore{"cache0" : dcache.NewKVStore("kv0"), 
                                 "cache1" : dcache.NewKVStore("kv1"), 
                                 "cache2" : dcache.NewKVStore("kv2"), 
                                 "cache3" : dcache.NewKVStore("kv3"), 
                                 "cache4" : dcache.NewKVStore("kv4"), 
                                 "cache5" : dcache.NewKVStore("kv5")}
  ports := [...]string{"6379", "6380", "6381", "6382", "6383", "6384"}
  nodes := [...]*dcache.HashRingNode {dcache.NewHashRingNode("node0", caches["cache0"]), 
                                      dcache.NewHashRingNode("node1", caches["cache1"]),
                                      dcache.NewHashRingNode("node2", caches["cache2"]),
                                      dcache.NewHashRingNode("node3", caches["cache3"]),
                                      dcache.NewHashRingNode("node4", caches["cache4"]),
                                      dcache.NewHashRingNode("node5", caches["cache5"])}
  server0 := dcache.NewServer("127.0.0.1", ports[0], nodes[0])
  server0.Start()
/*
  serverNames := make([]string, 0, len(nodes))
  for name, _ := range nodes {
    serverNames = append(serverNames, name )
  }
  sort.Strings(serverNames)
  ring := &dcache.HashRing{}
  for i, name := range serverNames {
    ring.AddNode(name, nodes[name])
  }

  data := map[string]string{ "key0" : "value0", 
                            "key1" : "value1", 
                            "key3" : "abhi", 
                            "key4" : "mano", 
                            "key5" : "khush", 
                            "key6" : "lov", 
                            "key7" : "mano", 
                            "key8" : "khush", 
                            "key11" : "value1", 
                            "key13" : "abhi", 
                            "key14" : "mano", 
                            "key15" : "khush", 
                            "key16" : "lov", 
                            "key17" : "mano", 
                            "key18" : "khush", 
                            "server5" : "lov", 
                            "server4" : "bush",
                            "server0" : "bush",
                            "server1" : "bush",
                            "server2" : "bush",
                            "server3" : "bush"}
  
  for key, value := range data {
    ring.Set(key, value)
  }
  for key, value := range data {
    fetchedValue := ring.Get(key)
    if fetchedValue !=  value {
      panic("value mismatch")
    }
  }
  totalKeys := ring.DbSize()
  if totalKeys != int64(len(data)) {
    panic("total keys size mismatch")
  }
*/
}
