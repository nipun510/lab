package dcache

import (
	context "context"
	"net"

	"google.golang.org/grpc"
	// Import path may need adjustment
)

type HashRingNodeGRPCServer struct {
	UnimplementedHashRingNodeServer
	node *HashRingNode
}

func NewHashRingNodeGRPCServer(node *HashRingNode) *HashRingNodeGRPCServer {
	return &HashRingNodeGRPCServer{node: node}
}

func (s *HashRingNodeGRPCServer) Set(ctx context.Context, req *KeyValue) (*SetResponse, error) {
	InfoLogger.Print("Received gRPC Set request for key: ", req.Key)
	err := s.node.Set(req.Key, req.Value)
	if err != nil {
		return &SetResponse{Ok: false, Error: err.Error()}, nil
	}
	return &SetResponse{Ok: true}, nil
}

func (s *HashRingNodeGRPCServer) Get(ctx context.Context, req *KeyRequest) (*GetResponse, error) {
	InfoLogger.Print("Received gRPC Get request for key: ", req.Key)
	value, found, err := s.node.Get(req.Key)
	return &GetResponse{Key: req.Key, Value: value, Found: found}, err
}

func (s *HashRingNodeGRPCServer) StartGRPCServer() error {
	lis, err := net.Listen("tcp", s.node.server.IP+":"+s.node.grpcPort)
	if err != nil {
		return err
	}
	grpcServer := grpc.NewServer()
	RegisterHashRingNodeServer(grpcServer, &HashRingNodeGRPCServer{node: s.node})
	InfoLogger.Printf("gRPC server listening on port %s", s.node.grpcPort)
	go grpcServer.Serve(lis)
	return nil
}
