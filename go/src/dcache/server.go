package dcache

import (
//  "encoding/json"
//  "io"
  "net/http"
  "fmt"
)

type Server struct {
  ip string
  port string
  node *HashRingNode
  ring *HashRing
}

func NewServer(ip, port string, node *HashRingNode) (*Server) {
  server := &Server{ip:ip, port:port, node:node}
  node.server = server
  server.ring = &HashRing{currNode:node}
  return server
}


type requestBody struct {
  Key string `json:"key"`
  Value string `json:"value"`
}

func Pong(w http.ResponseWriter, req *http.Request) {
  InfoLogger.Println("requestHandler : pong")
  fmt.Fprintf(w, "pong")
}

func (server *Server) FormRing(otherNodes []*HashRingNode) {
  for _, node := range otherNodes {
    server.ring.AddNode(node)
  }
}

func (server *Server) Start() {
  router := &Router{}
  router.handleFunc("/ping", Pong)
  socket := server.ip + ":" + server.port
  InfoLogger.Printf("Started http server at %s", socket) 
  http.HandleFunc("/", router.serveHTTP)
  http.ListenAndServe(socket, nil)
}
