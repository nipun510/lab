package mutility


type Flag struct {
  name string
  value string
}

type Command struct {
  name string
  description string
  flags []Flag
}


func runCliTest() {
}
