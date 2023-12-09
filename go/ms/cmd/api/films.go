
package main

import (
  "net/http"
  "ms.nipun.com/internal/data"
  "errors"
  "fmt"
)


func (app *application) showFilmsHandler (w http.ResponseWriter, r *http.Request) {
    title := r.URL.Query().Get("title")
    description := r.URL.Query().Get("description")
    year := r.URL.Query().Get("year")
    length := r.URL.Query().Get("length")
    sort := r.URL.Query().Get("sort")
    fmt.Println("value of sort is ", sort)
    
    filters := data.Filters{Sort : sort}
    films, err := app.models.Films.Get(title, description, year, length, filters)
    if err != nil {
      switch {
        case errors.Is(err, data.ErrRecordNotFound):
           app.notFoundResponse(w,r)
        default:
          app.serverErrorResponse(w, r, err)
      }
      return
    }

    err = app.writeJSON(w, http.StatusOK, envelope{"movies": films}, nil)
    if err != nil {
      app.logger.Println(err)
      http.Error(w, "The server encountered a problem and could not process your request", http.StatusInternalServerError)
    }
}
