package mutility

import (
  "crypto/sha1"
)

func ComputeSHA1Hash(str string) ([]byte) {
  h := sha1.New()
  h.Write([]byte(str))
  return h.Sum(nil)
}
