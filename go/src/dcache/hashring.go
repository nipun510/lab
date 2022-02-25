package dcache

import (
  "bytes"
  "mutility"
)



type HashRingNode struct {
  id string
  server *Server
  cache *KVStore
  hash []byte
  nextNode *HashRingNode
  prevNode *HashRingNode
}


func NewHashRingNode(id string, cache *KVStore) (*HashRingNode) {
  newNode := &HashRingNode{id:id, hash:mutility.ComputeSHA1Hash(id), cache: cache}
  return newNode
}

func (node *HashRingNode) DbSize() int64 {
  count :=  len(node.cache.data)
  InfoLogger.Printf("dbSize of node[%s] is [%d]", node.id, count)
  return int64(count)
}

type HashRing struct {
  startNode *HashRingNode
  currNode *HashRingNode
}

func (ring *HashRing) AddNode(newNode *HashRingNode) {
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
  return ring.findTargetNodeByHash(mutility.ComputeSHA1Hash(key))
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
  
func (ring *HashRing) Set(key, value string) {
  targetNode := ring.findTargetNodeByString(key)
  kvStore := targetNode.cache
  kvStore.Set(key, value)
  DebugLogger.Printf("key[%s] with hash[%x] is stored on node[%s]", key, mutility.ComputeSHA1Hash(key), targetNode.id)
}

func (ring *HashRing) Get(key string) (string) {
  targetNode := ring.findTargetNodeByString(key)
  kvStore := targetNode.cache
  DebugLogger.Printf("owner node for key[%s] is node[%s]", key, targetNode.id)
  return kvStore.Get(key)
}

func (ring *HashRing) DbSize() int64 {
  if ring.startNode == nil {

    return 0
  }
  var count int64
  currNode := ring.startNode
  if currNode != nil  {
    count = currNode.DbSize()
    currNode = currNode.nextNode
  }

  for ; currNode != ring.startNode; currNode = currNode.nextNode {
    count += currNode.DbSize()
  }
  InfoLogger.Printf("dbSize of ring is [%d]", count)
  return count
}


