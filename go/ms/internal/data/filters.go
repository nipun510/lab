package data

import (
  "strings"
)


type Filters struct {
  Page int
  PageSize int
  Sort string
}


func (f Filters) sortColumn() string {
  return strings.TrimPrefix(f.Sort, "-")
}

func (f Filters) sortDirection() string {
  if strings.HasPrefix(f.Sort, "-") {
    return "DESC"
  } else if len(f.Sort) > 0 {
    return "ASC"
  } else {
    return ""
  }
}

func (f Filters) orderByClause() {
  
}
