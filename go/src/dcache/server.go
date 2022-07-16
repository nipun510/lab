package dcache

import (
  "encoding/json"
  "net/http"
  "strings"
  "fmt"
)

// node is the one corresponding to this server
type Server struct {
  ip string
  port string
  internalPort string
  node *HashRingNode
}


type application struct {
  appServer *Server
}

var app application

func NewServer(ip, port, nodeId string) (*Server) {

  kvstore := NewKVStore(nodeId + "_kv")
  node := NewHashRingNode(nodeId, kvstore)
  ring := NewHashRing(node)
  node.ring = ring

  server := &Server{ip:ip, port:port, node:node}
  node.server = server
  return server
}

func (server *Server) CompleteRing(otherNodeIds []string) {
  for _, nodeId := range otherNodeIds {
    server.node.ring.AddNode(NewHashRingNode(nodeId, nil))
  }
}


func (app *application) Pong(w http.ResponseWriter, req *http.Request) {
  DebugLogger.Println("RequestHandler: Pong")
  fmt.Fprintf(w, "pong")
}

func (app *application) Set(w http.ResponseWriter, req *http.Request) {
  DebugLogger.Println("RequestHandler: Set")
  var input keyValue
  err := json.NewDecoder(req.Body).Decode(&input)
  if err != nil {
    ErrorLogger.Printf("Could not decode request body: %s", err)
    return
  }
  err = app.appServer.node.ring.Set(input.Key, input.Value)
  if err == nil {
    fmt.Fprintf(w, "OK")
  } else {
    ErrorLogger.Printf("Could not set key: %s", err)
    fmt.Fprintf(w, err.Error())
  }
}

func (app *application) Get(w http.ResponseWriter, req *http.Request) {
  DebugLogger.Println("RequestHandler: Get")
  var kv keyValue
  path := req.URL.Path
  kv.Key = strings.Split(path, "/")[2]
  value, err := app.appServer.node.ring.Get(kv.Key)
  if err != nil {
    http.Error(w, err.Error(), 404)
    return
  }
  kv.Value = value
  js, _ := json.Marshal(kv)
  w.Header().Set("Content-Type", "application/json")
  w.Write(js)
}




func (server *Server) Start() {
  app.appServer = server
  router := &Router{}
  router.HandleFunc(http.MethodGet, "/ping$",           app.Pong)
  router.HandleFunc(http.MethodGet, "/kv/\\w+$",        app.Get)
  router.HandleFunc(http.MethodPost,"/kv$",             app.Set)
  socket := server.ip + ":" + server.port
  InfoLogger.Printf("Started http server at %s", socket) 
  http.HandleFunc("/", router.ServeHTTP)
  http.ListenAndServe(socket, nil)
}
