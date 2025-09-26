package dcache

type ServerAddr struct {
	IP       string `json:"ip"`
	PORT     string `json:"port"`
	GRPCPORT string `json:"grpcPort"`
}

// node is the one corresponding to this server
type Server struct {
	ServerAddr
	node    *HashRingNode
	members map[string]ServerAddr // map of nodeId to grpcPort
}

func NewServer(ip, port, grpcPort, nodeId string) *Server {

	node := NewHashRingNode(nodeId, grpcPort)
	ring := NewHashRing(node)
	node.ring = ring

	server := &Server{ServerAddr: ServerAddr{IP: ip, PORT: port, GRPCPORT: grpcPort},
		node:    node,
		members: make(map[string]ServerAddr)}
	node.server = server

	return server
}

func (server *Server) AddMember(id string, ip, port, grpcPort string) {
	server.members[id] = ServerAddr{IP: ip, PORT: port, GRPCPORT: grpcPort}
}

func (server *Server) Start() {

	InfoLogger.Println("Starting server...")
	for id, addr := range server.members {
		memberNode := NewHashRingNode(id, addr.GRPCPORT)
		memberNode.server = &Server{ServerAddr: addr}
		server.node.ring.AddNode(memberNode)
	}

	err := server.node.startNode()
	if err != nil {
		ErrorLogger.Fatalf("Could not start node: %s", err)
	}

	InfoLogger.Printf("Started http server at %s", server.IP+":"+server.PORT)
	InfoLogger.Printf("Launching application...")
	app := NewApplication(server)
	app.Start()

}
