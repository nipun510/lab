package dcache

import (
	"strings"
	"time"

	"github.com/samuel/go-zookeeper/zk"
)

type ServerAddr struct {
	ID       string `json:"id"`
	IP       string `json:"ip"`
	PORT     string `json:"port"`
	GRPCPORT string `json:"grpc_port"`
}

// node is the one corresponding to this server
type Server struct {
	ServerAddr
	node    *HashRingNode
	zkConn  *zk.Conn
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
	// create members node in zookeeper if not exists
	conn, _, err := zk.Connect([]string{"localhost:2181"}, time.Second*5)
	if err != nil {
		ErrorLogger.Printf("Could not connect to Zookeeper: %v", err)
		return nil
	}
	defer conn.Close()

	path := "/members"
	_, err = conn.Create(path, []byte{}, 0, zk.WorldACL(zk.PermAll))
	if err != nil && err != zk.ErrNodeExists {
		ErrorLogger.Printf("Could not create members node in Zookeeper: %v", err)
	}
	return server
}

func (server *Server) AddMember(id, ip, port, grpcPort string) {
	server.members[id] = ServerAddr{ID: id, IP: ip, PORT: port, GRPCPORT: grpcPort}
	memberNode := NewHashRingNode(id, grpcPort)
	memberNode.server = &Server{ServerAddr: ServerAddr{ID: id, IP: ip, PORT: port, GRPCPORT: grpcPort}}
	server.node.ring.AddNode(memberNode)
}

func (server *Server) RemoveMember(id string) {
	delete(server.members, id)
}

func (server *Server) RemoveAllMembers() {
	server.members = make(map[string]ServerAddr)
}

func (server *Server) initZookeeper() {
	path := "/members"
	{
		var err error
		server.zkConn, _, err = zk.Connect([]string{"localhost:2181"}, time.Second*5)
		if err != nil {
			ErrorLogger.Printf("Could not connect to Zookeeper: %v", err)
			return
		}

		// if members node does not exist, create it
		_, err = server.zkConn.Create(path, []byte("testdata"), 0, zk.WorldACL(zk.PermAll))
		if err != nil && err != zk.ErrNodeExists {
			ErrorLogger.Printf("Could not create members node in Zookeeper: %v", err)
		}
		// add self to members
		memberPath := path + "/" + server.node.id
		data := server.node.id + ":" + server.IP + ":" + server.PORT + ":" + server.GRPCPORT
		memberPath, err = server.zkConn.Create(memberPath, []byte(data), zk.FlagEphemeral, zk.WorldACL(zk.PermAll))
		if err != nil && err != zk.ErrNodeExists {
			ErrorLogger.Printf("Could not create member node in Zookeeper: %v", err)
		}
		InfoLogger.Printf("Added self to Zookeeper members at %s", memberPath)
		// fetch all members
		children, _, err := server.zkConn.Children(path)
		if err != nil {
			ErrorLogger.Printf("Could not fetch members from Zookeeper: %v", err)
			return
		}

		for _, child := range children {
			data, _, err := server.zkConn.Get(path + "/" + child)
			if err != nil {
				ErrorLogger.Printf("Could not get member data from Zookeeper: %v", err)
				continue
			}
			parts := strings.Split(string(data), ":")
			if len(parts) != 4 {
				ErrorLogger.Printf("Invalid member data from Zookeeper: %v", string(data))
				continue
			}
			server.AddMember(parts[0], parts[1], parts[2], parts[3]) // id, ip, port, grpcPort
		}
	}

	// start a goroutine to watch for changes in members
	go func() {

		for {
			_, _, ch, err := server.zkConn.ChildrenW(path)
			if err != nil {
				ErrorLogger.Printf("Could not watch members in Zookeeper: %v", err)
				time.Sleep(time.Second * 5)
				continue
			}
			// watch for changes in member nodes
			<-ch
			// member list has changed
			children, _, err := server.zkConn.Children(path)
			if err != nil {
				ErrorLogger.Printf("Could not fetch members from Zookeeper: %v", err)
				continue
			}
			server.node.ring.resetNodes()
			// update server members
			server.members = make(map[string]ServerAddr)
			for _, child := range children {
				data, _, err := server.zkConn.Get(path + "/" + child)
				if err != nil {
					ErrorLogger.Printf("Could not get member data from Zookeeper: %v", err)
					continue
				}
				parts := strings.Split(string(data), ":")
				if len(parts) != 4 {
					ErrorLogger.Printf("Invalid member data from Zookeeper: %v", string(data))
					continue
				}
				server.AddMember(parts[0], parts[1], parts[2], parts[3]) // id, ip, port, grpcPort
			}
		}
	}()

}

func (server *Server) Start() {

	InfoLogger.Println("Starting server...")
	err := server.node.startNode()
	if err != nil {
		ErrorLogger.Fatalf("Could not start node: %s", err)
	}

	InfoLogger.Printf("Started http server at %s", server.IP+":"+server.PORT)

	server.initZookeeper()
	InfoLogger.Printf("Launching application...")
	app := NewApplication(server)
	app.Start()

}
