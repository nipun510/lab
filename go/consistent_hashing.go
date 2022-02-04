package main

import (
  "crypto/sha1"
  "bytes"
  "log"
  "os"
  "io/ioutil"
  "sort"
)

var (
    InfoLogger    *log.Logger
    DebugLogger   *log.Logger
    WarningLogger *log.Logger
    ErrorLogger   *log.Logger
)

func init() {
    DebugLogger   = log.New(os.Stdout, "DEBUG: ", log.Ldate|log.Ltime|log.Lshortfile)
    InfoLogger    = log.New(os.Stdout, "INFO: ", log.Ldate|log.Ltime|log.Lshortfile)
    WarningLogger = log.New(os.Stdout, "WARNING: ", log.Ldate|log.Ltime|log.Lshortfile)
    ErrorLogger   = log.New(os.Stdout, "ERROR: ", log.Ldate|log.Ltime|log.Lshortfile)
    DebugLogger.SetOutput(ioutil.Discard)
}


func computeSHA1Hash(str string) ([]byte) {
  h := sha1.New()
  h.Write([]byte(str))
  return h.Sum(nil)
}


type KVStore struct {
  id string
  data map[string]string
}

func NewKVStore (id string) *KVStore {
  store := &KVStore{}
  store.data = make(map[string]string)
  store.id = id
  return store
}


func (store *KVStore) set(key, value string) {
  store.data[key] = value
}

func (store *KVStore) get(key string) (string) {
  return store.data[key]
}


type HashRingNode struct {
  id string
  server *KVStore
  hash []byte
  nextNode *HashRingNode
  prevNode *HashRingNode
}

func (node *HashRingNode) dbSize() int64 {
  count :=  len(node.server.data)
  InfoLogger.Printf("dbSize of node[%s] is [%d]", node.id, count)
  return int64(count)
}

type HashRing struct {
  startNode *HashRingNode
}

func (ring *HashRing) addNode(id string, server *KVStore) {
  newNode := &HashRingNode{id, server, computeSHA1Hash(id), nil, nil}

  if ring.startNode == nil {
    ring.startNode = newNode
    newNode.nextNode = newNode
    newNode.prevNode = newNode
    InfoLogger.Printf("node[%s] added to ring with id[%x]", newNode.id, newNode.hash)
    return
  }

  targetNode := ring.findTargetNodeByHash(newNode.hash)
  tmp := targetNode.prevNode
  targetNode.prevNode = newNode
  tmp.nextNode = newNode
  newNode.nextNode = targetNode
  newNode.prevNode = tmp
  InfoLogger.Printf("node[%s] added to ring with id[%x]", newNode.id, newNode.hash)
}

func (ring *HashRing) findTargetNodeByString(key string) (*HashRingNode){
  return ring.findTargetNodeByHash(computeSHA1Hash(key))
}

func (ring *HashRing) findTargetNodeByHash(keyHash []byte) (*HashRingNode){
  if ring.startNode == nil {
    return nil
  }

  for currNode := ring.startNode.nextNode; currNode != ring.startNode; 
                                          currNode = currNode.nextNode {
    if (bytes.Compare(currNode.hash, keyHash) >= 0) && 
       (bytes.Compare(currNode.prevNode.hash, keyHash) < 0) {
       return currNode
    }
    if bytes.Compare(currNode.hash, currNode.prevNode.hash) < 0 {
      if (bytes.Compare(currNode.hash, keyHash) >= 0) ||
         (bytes.Compare(currNode.prevNode.hash, keyHash) < 0) {
           return currNode
      }
    }
  }
  // keyHash value doesn't lies between any two existing nodes
  return ring.startNode
}
  
func (ring *HashRing) set(key, value string) {
  targetNode := ring.findTargetNodeByString(key)
  targetServer := targetNode.server
  targetServer.set(key, value)
  DebugLogger.Printf("key[%s] with hash[%x] is stored on node[%s]", key, computeSHA1Hash(key), targetNode.id)
}

func (ring *HashRing) get(key string) (string) {
  targetNode := ring.findTargetNodeByString(key)
  targetServer := targetNode.server
  DebugLogger.Printf("owner node for key[%s] is node[%s]", key, targetNode.id)
  return targetServer.get(key)
}

func (ring *HashRing) dbSize() int64 {
  if ring.startNode == nil {

    return 0
  }
  var count int64
  currNode := ring.startNode
  if currNode != nil  {
    count = currNode.dbSize()
    currNode = currNode.nextNode
  }

  for ; currNode != ring.startNode; currNode = currNode.nextNode {
    count += currNode.dbSize()
  }
  InfoLogger.Printf("dbSize of ring is [%d]", count)
  return count
}

func main() {
  servers := map[string]*KVStore{"node0" : NewKVStore("kv0"), 
                                 "node1" : NewKVStore("kv1"), 
                                 "node2" : NewKVStore("kv2"), 
                                 "node3" : NewKVStore("kv3"), 
                                 "node4" : NewKVStore("kv4"), 
                                 "node5" : NewKVStore("kv5")}
  serverNames := make([]string, 0, len(servers))
  for name, _ := range servers {
    serverNames = append(serverNames, name)
  }
  sort.Strings(serverNames)

  ring := &HashRing{}
  for _, name := range serverNames {
    ring.addNode(name, servers[name])
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
    ring.set(key, value)
  }
  for key, value := range data {
    fetchedValue := ring.get(key)
    if fetchedValue !=  value {
      panic("value mismatch")
    }
  }
  totalKeys := ring.dbSize()
  if totalKeys != int64(len(data)) {
    panic("total keys size mismatch")
  }
}
