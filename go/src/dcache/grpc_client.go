package dcache

import (
	"time"

	"google.golang.org/grpc" // Import path may need adjustment
)

func NewHashRingNodeGRPCClient(addr string) (HashRingNodeClient, error) {
	conn, err := grpc.Dial(addr, grpc.WithInsecure(), grpc.WithBlock(), grpc.WithTimeout(2*time.Second))
	if err != nil {
		return nil, err
	}
	return NewHashRingNodeClient(conn), nil
}
