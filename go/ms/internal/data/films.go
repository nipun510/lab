
package data

import (
  "database/sql"
  "context"
  "time"
  "fmt"
)


type Film struct {
  FilmId int64
  Title string
  Description string
  Year int32
  Length int32
}

type FilmModel struct{
  DB *sql.DB
}

type fieldsNameMap map[string]string

func newFieldsNameMap()  fieldsNameMap {
  fieldsName := map[string]string{
    "" : "",
    "FilmdId" : "film_id", "filmId"  : "film_id",
    "Title"   : "title", "title" : "title",
    "Description" : "description", "description" : "description",
    "Year" : "release_year", "year" : "release_year",
    "Length" : "length", "length" : "length"}
  return fieldsName
}

var fieldsMapping fieldsNameMap = newFieldsNameMap()

func (m FilmModel) Get(title string, description string, year string, length string, filters Filters) ([]Film, error) {
    sortQuery := " film_id ASC" 
    if len(filters.sortColumn()) > 0 {
      sortQuery =  fieldsMapping[filters.sortColumn()] + " " + filters.sortDirection()
      sortQuery = sortQuery + ", film_id ASC"
    }
    query := fmt.Sprintf(`
        SELECT film_id, title, description, release_year, length 
        FROM film
        WHERE (to_tsvector('simple', title) @@ plainto_tsquery('simple', $1) OR $1 = '')
        AND  (to_tsvector('simple', description) @@ plainto_tsquery('simple', $2) OR $2 = '')
        AND (release_year = TO_NUMBER($3, '9999999999.99') OR $3 = '')
        AND  (length = TO_NUMBER($4, '9999999999.99') OR $4 = '')
        ORDER BY %s`, sortQuery)
    fmt.Println(query)

     ctx, cancel := context.WithTimeout(context.Background(), 3*time.Second)
     defer cancel()


    rows, err := m.DB.QueryContext(ctx, query, title, description, year, length)
    if err != nil {
      return nil, err
    }

    defer rows.Close()

    var films []Film 
    for rows.Next() {
      var tmpFilm Film
      if err := rows.Scan(
        &tmpFilm.FilmId,
        &tmpFilm.Title,
        &tmpFilm.Description,
        &tmpFilm.Year,
        &tmpFilm.Length); err != nil {
        return films, err
      }
      films = append(films, tmpFilm)
    }
    if err:= rows.Err(); err != nil {
      return films, err
    }
    return films, nil
}
