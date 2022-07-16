package dcache

type keyValue struct {
  Key string `json:"key"`
  Value string `json:"value"`
}

type KVStore struct {
  id string
  data map[string]string
}

func NewKVStore (id string) *KVStore {
  store := &KVStore{}
  store.data = make(map[string]string)
  store.id = id
  return store
}


func (store *KVStore) Set(key, value string) {
  store.data[key] = value
}

func (store *KVStore) Get(key string) (string) {
  return store.data[key]
}
