package dcache

import (
	"bytes"
	"errors"

	"lab.com/mutility"
)

type HashRingNode struct {
	id       string
	server   *Server
	cache    *KVStore
	ring     *HashRing
	hash     []byte
	nextNode *HashRingNode
	prevNode *HashRingNode
}

func NewHashRingNode(id string, cache *KVStore) *HashRingNode {
	// ring, nextNode, and prevNode will be set when the node is added to a ring
	newNode := &HashRingNode{
		id:    id,
		hash:  mutility.ComputeSHA1Hash(id),
		cache: cache,
	}
	return newNode
}

func (node *HashRingNode) DbSize() int64 {
	count := len(node.cache.data)
	InfoLogger.Printf("dbSize of node[%s] is [%d]", node.id, count)
	return int64(count)
}

type HashRing struct {
	startNode *HashRingNode
}

func NewHashRing(node *HashRingNode) *HashRing {
	ring := &HashRing{startNode: node}
	ring.startNode.ring = ring
	ring.startNode.nextNode = ring.startNode
	ring.startNode.prevNode = ring.startNode
	InfoLogger.Printf("ring formed having node[%s] with id[%x]", ring.startNode.id, ring.startNode.hash)
	return ring
}

func (ring *HashRing) AddNode(newNode *HashRingNode) {
	if ring.startNode == nil {
		ring.startNode.ring = ring
		ring.startNode.nextNode = ring.startNode
		ring.startNode.prevNode = ring.startNode
		InfoLogger.Printf("node[%s] added to ring with id[%x]", ring.startNode.id, ring.startNode.hash)
		return
	}

	targetNode := ring.findTargetNodeByHash(newNode.hash)
	prevNode := targetNode.prevNode
	targetNode.prevNode = newNode
	prevNode.nextNode = newNode
	newNode.nextNode = targetNode
	newNode.prevNode = prevNode
	newNode.ring = ring
	InfoLogger.Printf("node[%s] added to ring with id[%x]", newNode.id, newNode.hash)
}

func (ring *HashRing) findTargetNodeByString(key string) *HashRingNode {
	return ring.findTargetNodeByHash(mutility.ComputeSHA1Hash(key))
}

// It finds next node in clockwise direction in the ring for the given keyhash.
func (ring *HashRing) findTargetNodeByHash(keyHash []byte) *HashRingNode {
	if ring.startNode == nil {
		return nil
	}

	for currNode := ring.startNode.nextNode; currNode != ring.startNode; currNode = currNode.nextNode {
		if (bytes.Compare(currNode.hash, keyHash) >= 0) &&
			(bytes.Compare(currNode.prevNode.hash, keyHash) < 0) {
			return currNode
		}
		// Handle the wrap-around case where the hash ring wraps from the highest hash value back to the lowest.
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

func (ring *HashRing) getAllNodesAddress() ([]string, error) {
	var addressList []string
	if ring.startNode == nil {
		return addressList, nil
	}

	for currNode := ring.startNode.nextNode; currNode != ring.startNode; currNode = currNode.nextNode {
		addressList = append(addressList,
			currNode.server.ip+":"+currNode.server.port)
	}
	return addressList, nil
}

func (ring *HashRing) Set(key, value string) error {
	targetNode := ring.findTargetNodeByString(key)
	if targetNode != ring.startNode {
		InfoLogger.Printf("key[%s] is not present on node[%s], correct destination node[%s]", key, ring.startNode.id, targetNode.id)
		return errors.New("Wrong Node")
	}
	kvStore := targetNode.cache
	kvStore.Set(key, value)
	DebugLogger.Printf("key[%s] with hash[%x] is stored on node[%s]", key, mutility.ComputeSHA1Hash(key), targetNode.id)
	return nil
}

func (ring *HashRing) Get(key string) (string, error) {
	targetNode := ring.findTargetNodeByString(key)
	if targetNode != ring.startNode {
		InfoLogger.Printf("key[%s] is not present on node[%s], correct destination node[%s]", key, ring.startNode.id, targetNode.id)
		return "", errors.New("Wrong Node")
	}
	kvStore := targetNode.cache
	DebugLogger.Printf("owner node for key[%s] is node[%s]", key, targetNode.id)
	return kvStore.Get(key), nil
}

func (ring *HashRing) DbSize() int64 {
	if ring.startNode == nil {

		return 0
	}
	var count int64
	currNode := ring.startNode
	if currNode != nil {
		count = currNode.DbSize()
		currNode = currNode.nextNode
	}

	for ; currNode != ring.startNode; currNode = currNode.nextNode {
		count += currNode.DbSize()
	}
	InfoLogger.Printf("dbSize of ring is [%d]", count)
	return count
}
