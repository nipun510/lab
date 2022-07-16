package dcache

import (
  "net/http"
  "regexp"
)

type Route struct {
    method string
    pattern *regexp.Regexp
    handler http.Handler
}

func NewRoute(method string, pattern *regexp.Regexp, handler http.Handler) (*Route)  {
  return &Route{method, pattern, handler}
}

type Router struct {
    routes []*Route
}

func (h *Router) handler(method, pattern string, handler http.Handler) {
    regex, _ := regexp.Compile(pattern)
    h.routes = append(h.routes, NewRoute(method, regex, handler))
}

func (h *Router) HandleFunc(method, pattern string, handler func(http.ResponseWriter, *http.Request)) {
    regex, _ := regexp.Compile(pattern)
    h.routes = append(h.routes, NewRoute(method, regex, http.HandlerFunc(handler)))
}

func (h *Router) ServeHTTP(w http.ResponseWriter, r *http.Request) {
    InfoLogger.Printf("HTTP request method[%s], URL[%s]", r.Method, r.URL.Path)
    for _, route := range h.routes {
        if route.pattern.MatchString(r.URL.Path) && route.method == r.Method {
            route.handler.ServeHTTP(w, r)
            return
        }
    }
    // no pattern matched; send 404 response
    http.NotFound(w, r)
}
