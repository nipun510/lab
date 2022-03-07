#include <vector>
#include <cassert>


struct node {
  int rank{0};
  int parent{0};
};

class disjointSet {
  public:
  disjointSet(int n) : _sets(n) {
    for (int i = 0; i < n; ++i) {
      _sets[i].parent = i;
    }
  }
  int find(int p);
  void unite(int p, int q);

  private:
  std::vector<node> _sets;
};

int
disjointSet::find(int x) 
{
  if (x != _sets[x].parent) {
    _sets[x].parent = find(_sets[x].parent);
  }
  return _sets[x].parent;
}

void
disjointSet::unite(int x, int y) 
{
  int xroot = find(x);
  int yroot = find(y);
  if (xroot != yroot) {
    if (_sets[xroot].rank == _sets[yroot].rank) {
      _sets[yroot].parent = xroot;
      _sets[xroot].rank += 1;
    } else if (_sets[xroot].rank > _sets[yroot].rank) {
      _sets[yroot].parent = xroot;
    } else {
      _sets[xroot].parent = yroot;
    }
  }
}

int main() {
  disjointSet ds(10);
  ds.unite(1, 3);
  assert(ds.find(1) == ds.find(3));
  assert(ds.find(1) != ds.find(2));
}
