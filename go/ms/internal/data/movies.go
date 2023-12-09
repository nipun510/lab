package data

import (
  "time"
  "errors"
  "context"
  "database/sql"
  "github.com/lib/pq"
)


type Movie struct {
  ID int64
  CreatedAt time.Time
  Title string
  Year int32
  Runtime int32
  Genres []string
  Version int32
}

type MovieModel struct{
  DB *sql.DB
}

func (m MovieModel) Insert(movie *Movie) error {
  query := `
   INSERT INTO movies(title, year, runtime, genres)
   VALUES($1, $2, $3, $4)
   RETURNING id, created_at, version`



   pqGenres := pq.Array(movie.Genres)
   args := []interface{}{movie.Title, movie.Year, movie.Runtime, pqGenres}

   ctx, cancel := context.WithTimeout(context.Background(), 3*time.Second)
   defer cancel()

   return m.DB.QueryRowContext(ctx, query, args...).Scan(&movie.ID, &movie.CreatedAt, &movie.Version)
}

func (m MovieModel) Get(id int64) (*Movie, error) {
  query := `
    SELECT id, created_at, title, year, runtime, genres, version 
    FROM movies 
    where id = $1`
    var movie Movie

    ctx, cancel := context.WithTimeout(context.Background(), 3*time.Second)
    defer cancel()

    err := m.DB.QueryRowContext(ctx, query, id).Scan(
      &movie.ID,
      &movie.CreatedAt,
      &movie.Title,
      &movie.Year,
      &movie.Runtime,
      pq.Array(&movie.Genres),
      &movie.Version)

   if err != nil {
     switch {
       case errors.Is(err, sql.ErrNoRows):
           return nil, ErrRecordNotFound
       default:
         return nil, err
     }
   }

   return &movie, nil
}

func (m MovieModel) Update(movie *Movie) error {
  query := `
    UPDATE movies
    SET title = $1, year = $2, runtime = $3, genres = $4, version = version + 1
    WHERE id = $5 AND version = $6
    RETURNING version`

  args := []interface {} {
    movie.Title,
    movie.Year,
    movie.Runtime,
    pq.Array(movie.Genres),
    movie.ID,
    movie.Version,
  }

  ctx, cancel := context.WithTimeout(context.Background(), 3*time.Second)
  defer cancel()
  return m.DB.QueryRowContext(ctx, query, args...).Scan(&movie.Version)
}


func (m MovieModel) Delete(id int64) error {
  if id < 1 {
    return ErrRecordNotFound
  }

  query := `
    DELETE FROM movies
    WHERE id = $1`

  ctx, cancel := context.WithTimeout(context.Background(), 3*time.Second)
  defer cancel()

  result, err := m.DB.ExecContext(ctx, query, id)
  if err != nil {
    return err
  }
  rowsAffected, err := result.RowsAffected()

  if rowsAffected == 0{
    return ErrEditConflict
  }
  return nil
}
