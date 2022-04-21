

// not a meaning concept, concept should have a semantic value
template<typename T>
concept Doable = requires (T t) {
  { t.do_it()};
};
