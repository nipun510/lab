#include <vector>
#include <cassert>


struct node {
  int rank{0};
  int parent{0};
};

class disjointSet {
  public:
  disjointSet(int n) : _subsets(n) {
    for (int i = 0; i < n; ++i) {
      _subsets[i].parent = i;
    }
  }
  int find(int p);
  void unite(int p, int q);

  private:
  std::vector<node> _subsets;
};

/*
   index x denotes a node in a subset.
   find returns the root of this subset.
   root node is reprensented by an index whose parent is itself.
   compression happening below reduces the height of tree.
*/
int
disjointSet::find(int x) 
{
  if (x != _subsets[x].parent) {
    _subsets[x].parent = find(_subsets[x].parent);
  }
  return _subsets[x].parent;
}

/*
   unite combines subset with x as member and subset with y as member node.
*/
void
disjointSet::unite(int x, int y) 
{
  int xroot = find(x);
  int yroot = find(y);
  if (xroot != yroot) {
    if _subsets_sets[xroot].rank == _subsets[yroot].rank) {
      _subsets[yroot].parent = xroot;
      _subsets[xroot].rank += 1;
    } else if (_subsets[xroot].rank > _subsets[yroot].rank) {
      _subsets[yroot].parent = xroot;
    } else {
      _subsets[xroot].parent = yroot;
    }
  }
}

int main() {
  disjointSet ds(10);
  ds.unite(1, 3);
  assert(ds.find(1) == ds.find(3));
  assert(ds.find(1) != ds.find(2));
}
