package dcache

import (
	"encoding/json"
	"fmt"
	"net/http"
)

type application struct {
	appServer *Server
	router    *Router
}

func NewApplication(server *Server) *application {
	return &application{appServer: server, router: &Router{}}
}

func (app *application) configureRouter() {
	app.router.HandleFunc(http.MethodGet, "/ping$", app.Pong)
	app.router.HandleFunc(http.MethodGet, "/kv$", app.Get)
	app.router.HandleFunc(http.MethodPost, "/kv$", app.Set)
}

func (app *application) Start() {
	app.configureRouter()
	http.HandleFunc("/", app.router.ServeHTTP)
	InfoLogger.Printf("HTTP server listening on %s:%s", app.appServer.IP, app.appServer.PORT)
	err := http.ListenAndServe(app.appServer.IP+":"+app.appServer.PORT, nil)
	if err != nil {
		ErrorLogger.Fatalf("Could not start http server: %s", err)
	}
}

func (app *application) Pong(w http.ResponseWriter, req *http.Request) {
	DebugLogger.Println("RequestHandler: Pong")
	fmt.Fprintf(w, "pong")
}

func (app *application) Set(w http.ResponseWriter, req *http.Request) {
	DebugLogger.Println("RequestHandler: Set")
	var kv keyValue

	// Parse key and value from query parameters
	query := req.URL.Query()
	kv.Key = query.Get("key")
	kv.Value = query.Get("value")

	if kv.Key == "" || kv.Value == "" {
		http.Error(w, "Missing 'key' or 'value' query parameter", http.StatusBadRequest)
		return
	}

	err := app.appServer.node.ring.Set(kv.Key, kv.Value)
	if err != nil {
		ErrorLogger.Printf("Could not set key: %s", err)
	}
}

func (app *application) Get(w http.ResponseWriter, req *http.Request) {
	DebugLogger.Println("RequestHandler: Get")
	var kv keyValue

	// Parse key and value from query parameters
	query := req.URL.Query()
	kv.Key = query.Get("key")

	if kv.Key == "" {
		ErrorLogger.Printf("Missing 'key' query parameter")
		http.Error(w, "Missing 'key' query parameter", http.StatusBadRequest)
		return
	}

	value, found, err := app.appServer.node.ring.Get(kv.Key)
	if err != nil {
		ErrorLogger.Printf("Could not get key: %s", err)
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
	if !found {
		http.Error(w, "Key not found", http.StatusNotFound)
		return
	}
	kv.Value = value
	js, _ := json.Marshal(kv)
	w.Header().Set("Content-Type", "application/json")
	w.WriteHeader(http.StatusOK)
	w.Write(js)
}
