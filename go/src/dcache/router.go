package dcache

import (
  "net/http"
  "regexp"
)

type route struct {
    pattern *regexp.Regexp
    handler http.Handler
}

type Router struct {
    routes []*route
}

func (h *Router) handler(pattern string, handler http.Handler) {
    regex, _ := regexp.Compile(pattern)
    h.routes = append(h.routes, &route{regex, handler})
}

func (h *Router) handleFunc(pattern string, handler func(http.ResponseWriter, *http.Request)) {
    regex, _ := regexp.Compile(pattern)
    h.routes = append(h.routes, &route{regex, http.HandlerFunc(handler)})
}

func (h *Router) serveHTTP(w http.ResponseWriter, r *http.Request) {
    InfoLogger.Printf("Got request from url %s", r.URL.Path)
    for _, route := range h.routes {
        if route.pattern.MatchString(r.URL.Path) {
            route.handler.ServeHTTP(w, r)
            return
        }
    }
    // no pattern matched; send 404 response
    http.NotFound(w, r)
}
