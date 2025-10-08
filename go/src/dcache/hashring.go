package dcache

import (
	"bytes"
	"context"
	"errors"

	"lab.com/mutility"
)

type HashRingNode struct {
	id         string
	server     *Server
	cache      *KVStore
	ring       *HashRing
	grpcPort   string
	grpcServer *HashRingNodeGRPCServer
	hash       []byte
	nextNode   *HashRingNode
	prevNode   *HashRingNode
}

func NewHashRingNode(id string, grpcPort string) *HashRingNode {
	// ring, nextNode, and prevNode will be set when the node is added to a ring
	newNode := &HashRingNode{
		id:         id,
		grpcPort:   grpcPort,
		grpcServer: nil, // Temporary nil, will set below
		hash:       mutility.ComputeSHA1Hash(id),
		cache:      NewKVStore(id + "_kv"),
	}
	newNode.grpcServer = NewHashRingNodeGRPCServer(newNode)
	return newNode
}

func (node *HashRingNode) startNode() error {
	err := node.grpcServer.StartGRPCServer()
	if err != nil {
		ErrorLogger.Printf("Failed to start gRPC server: %v", err)
		return err
	}
	InfoLogger.Printf("Started Node[%s] with id[%x]", node.id, node.hash)
	return nil
}

func (node *HashRingNode) DbSize() int64 {
	count := len(node.cache.data)
	InfoLogger.Printf("dbSize of node[%s] is [%d]", node.id, count)
	return int64(count)
}

func (node *HashRingNode) Set(key, value string) error {
	InfoLogger.Printf("Setting key[%s] to value[%s] in node[%s] cacheStore", key, value, node.id)
	node.cache.Set(key, value)
	DebugLogger.Printf("key[%s] with hash[%x] is stored on node[%s]", key, mutility.ComputeSHA1Hash(key), node.id)
	return nil
}

func (node *HashRingNode) Get(key string) (string, bool, error) {
	InfoLogger.Printf("Getting key[%s] from node[%s] cacheStore", key, node.id)
	value, ok := node.cache.Get(key)
	return value, ok, nil
}

type HashRing struct {
	startNode *HashRingNode
}

func NewHashRing(node *HashRingNode) *HashRing {
	ring := &HashRing{startNode: node}
	ring.startNode.ring = ring
	ring.startNode.nextNode = ring.startNode
	ring.startNode.prevNode = ring.startNode
	return ring
}

func (ring *HashRing) AddNode(newNode *HashRingNode) {
	if ring.startNode == nil {
		ring.startNode.ring = ring
		ring.startNode.nextNode = ring.startNode
		ring.startNode.prevNode = ring.startNode
		InfoLogger.Printf("Added node[%s] to ring with id[%x]", ring.startNode.id, ring.startNode.hash)
		return
	}

	targetNode := ring.findTargetNodeByHash(newNode.hash)
	prevNode := targetNode.prevNode
	targetNode.prevNode = newNode
	prevNode.nextNode = newNode
	newNode.nextNode = targetNode
	newNode.prevNode = prevNode
	newNode.ring = ring
	InfoLogger.Printf("Added node[%s] to ring with id[%x]", newNode.id, newNode.hash)
}

func (ring *HashRing) resetNodes() {
	ring.startNode.nextNode = ring.startNode
	ring.startNode.prevNode = ring.startNode
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

func (ring *HashRing) getNodesInfo() []ServerAddr {
	// return slice of ServerAddr
	var nodesInfo []ServerAddr
	if ring.startNode == nil {
		return nodesInfo
	}
	currNode := ring.startNode
	nodesInfo = append(nodesInfo, ServerAddr{ID: currNode.id, IP: currNode.server.IP, PORT: currNode.server.PORT, GRPCPORT: currNode.grpcPort})
	currNode = currNode.nextNode

	for ; currNode != ring.startNode; currNode = currNode.nextNode {
		nodesInfo = append(nodesInfo, ServerAddr{ID: currNode.id, IP: currNode.server.IP, PORT: currNode.server.PORT, GRPCPORT: currNode.grpcPort})
	}
	return nodesInfo
}

func (ring *HashRing) Set(key, value string) error {
	targetNode := ring.findTargetNodeByString(key)
	if targetNode != ring.startNode {
		InfoLogger.Printf("Forwarding Set request for key[%s] to node[%s] at %s:%s", key, targetNode.id, targetNode.server.IP, targetNode.grpcPort)
		grpcClient, err := NewHashRingNodeGRPCClient(targetNode.server.IP + ":" + targetNode.grpcPort)
		if err != nil {
			InfoLogger.Printf("Failed to create gRPC client for node[%s] at %s:%s", targetNode.id, targetNode.server.IP, targetNode.grpcPort)
			return err
		}
		_, err = grpcClient.Set(context.Background(), &KeyValue{Key: key, Value: value})
		if err != nil {
			InfoLogger.Printf("gRPC Set request to node[%s] failed: %v", targetNode.id, err)
			return err
		}
		return nil
	}
	return targetNode.Set(key, value)
}

func (ring *HashRing) Get(key string) (string, bool, error) {
	targetNode := ring.findTargetNodeByString(key)
	if targetNode == ring.startNode {
		return targetNode.Get(key)
	}

	InfoLogger.Printf("key[%s] is not present on this node. Forwarding request to node[%s]", key, targetNode.id)
	grpcClient, err := NewHashRingNodeGRPCClient(targetNode.server.IP + ":" + targetNode.grpcPort)
	if err != nil {
		InfoLogger.Printf("Failed to create gRPC client for node[%s] at %s:%s", targetNode.id, targetNode.server.IP, targetNode.grpcPort)
		return "", false, err
	}
	resp, err := grpcClient.Get(context.Background(), &KeyRequest{Key: key})
	if err != nil {
		InfoLogger.Printf("gRPC Get request to node[%s] failed: %v", targetNode.id, err)
		return "", false, err
	}
	if resp == nil {
		return "", false, errors.New("received nil response from gRPC Get")
	}
	return resp.Value, resp.Found, nil
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
