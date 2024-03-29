package main

import (
  "fmt"
  "net/http"
  "ms.nipun.com/internal/data"
  "errors"
)

func (app *application) createMovieHandler (w http.ResponseWriter, r *http.Request) {
  var input struct {
    Title string `json:"title"`
    Year int32 `json:"year"`
    Runtime int32 `json:"runtime"`
    Genres []string `json:"genres"`
  }

  err := app.readJSON(w, r, &input)
  if err != nil {
    app.badRequestResponse(w, r, err)
    return
  }
  //fmt.Fprintf(w, "%+v\n", input)
  movie := data.Movie{
    Title: input.Title,
    Year: input.Year,
    Runtime: input.Runtime,
    Genres: input.Genres,
  }


    err = app.models.Movies.Insert(&movie)
    if err != nil {
      app.serverErrorResponse(w, r, err)
      return
    }
    headers := make(http.Header)
    headers.Set("Location", fmt.Sprintf("/v1/movies/id", movie.ID))

    err = app.writeJSON(w, http.StatusCreated, envelope{"movie": movie}, headers)
    if err != nil {
      app.serverErrorResponse(w, r, err)
    }
}


func (app *application) showMovieHandler (w http.ResponseWriter, r *http.Request) {
    id, err := app.readIDParam(r)
    if err != nil {
      http.NotFound(w, r)
      return
    }

    movie, err := app.models.Movies.Get(id)
    if err != nil {
      switch {
        case errors.Is(err, data.ErrRecordNotFound):
           app.notFoundResponse(w,r)
        default:
          app.serverErrorResponse(w, r, err)
      }
      return
    }

    err = app.writeJSON(w, http.StatusOK, envelope{"movie": *movie}, nil)
    if err != nil {
      app.logger.Println(err)
      http.Error(w, "The server encountered a problem and could not process your request", http.StatusInternalServerError)
    }
}

func (app *application) updateMovieHandler (w http.ResponseWriter, r *http.Request) {
  id, err := app.readIDParam(r)
  if err != nil {
    app.notFoundResponse(w, r)
    return
  }

  movie, err := app.models.Movies.Get(id)
  if err != nil {
    switch {
      case errors.Is(err, data.ErrRecordNotFound):
        app.notFoundResponse(w, r)
      default:
        app.serverErrorResponse(w, r, err)
    }
      return
  }
  var input struct {
    Title *string  `json:"title"`
    Year *int32    `json:"year"`
    Runtime *int32 `json:"runtime"`
    Genres []string    `json:"genres"`
  }

  err = app.readJSON(w, r, &input) 
  if err != nil {
    app.badRequestResponse(w, r, err)
    return
  }
  if input.Year != nil {
    movie.Year = *input.Year
  }
  if input.Runtime != nil {
    movie.Runtime = *input.Runtime
  }
  if input.Genres != nil {
    movie.Genres = input.Genres
  }

  err = app.models.Movies.Update(movie)
  if err != nil {
    switch {
      case errors.Is(err, data.ErrEditConflict):
        app.editConflictResponse(w, r)
      default:
        app.serverErrorResponse(w, r, err)
    }
    return
  }

  err = app.writeJSON(w, http.StatusOK, envelope{"movie" : movie}, nil)
  if err != nil {
    app.serverErrorResponse(w, r, err)
  }
}

func (app *application) deleteMovieHandler (w http.ResponseWriter, r *http.Request) {
  id, err := app.readIDParam(r)
  if err != nil {
    app.notFoundResponse(w, r)
    return
  }
  err = app.models.Movies.Delete(id)
  if err != nil {
    switch {
      case errors.Is(err, data.ErrRecordNotFound):
        app.notFoundResponse(w, r)
      default:
        app.serverErrorResponse(w, r, err)
    }
      return
  }

  err = app.writeJSON(w, http.StatusOK, envelope{"message" : fmt.Sprintf("movie with id %d deleted", id)}, nil)
  if err != nil {
    app.serverErrorResponse(w, r, err)
  }
}
