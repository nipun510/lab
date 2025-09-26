package dcache

type keyValue struct {
	Key   string `json:"key"`
	Value string `json:"value"`
}

type KVStore struct {
	id   string
	data map[string]string
}

func (store *KVStore) Exists(key string) bool {
	_, exists := store.data[key]
	return exists
}

func NewKVStore(id string) *KVStore {
	store := &KVStore{}
	store.data = make(map[string]string)
	store.id = id
	return store
}

func (store *KVStore) Set(key, value string) {
	store.data[key] = value
}

func (store *KVStore) Get(key string) (string, bool) {
	value, exists := store.data[key]
	return value, exists
}
