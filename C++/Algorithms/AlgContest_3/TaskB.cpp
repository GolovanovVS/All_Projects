#include <cmath>
#include <iostream>
#include <vector>

struct Segtree {
  std::vector<int> tree;
  int size;
  void Init(int number) {
    size = 1;
    while (size < number) {
      size *= 2;
    }
    tree.resize(4 * size);
  }
  void Upgrade(int ind, int val, int cur, int lef, int rig) {
    if (rig - lef == 1) {
      tree[cur] = val * (int)std::pow(-1, lef % 2);
      return;
    }
    int mid = (lef + rig) / 2;
    if (ind >= mid) {
      Upgrade(ind, val, 2 * cur + 2, mid, rig);
    } else {
      Upgrade(ind, val, 2 * cur + 1, lef, mid);
    }
    tree[cur] = tree[2 * cur + 1] + tree[2 * cur + 2];
  }

  int Get(int cur, int lef, int rig, int lbord, int rbord) {
    if (lbord >= rig || lef >= rbord) {
      return 0;
    }
    if (lbord <= lef && rig <= rbord) {
      return tree[cur];
    }
    int mid = (lef + rig) / 2;
    return Get(2 * cur + 1, lef, mid, lbord, rbord) +
           Get(2 * cur + 2, mid, rig, lbord, rbord);
  }

  void Build(std::vector<int>& vec, int ind, int lef, int rig) {
    if (rig - lef == 1) {
      tree[ind] = vec[lef] * (int)std::pow(-1, lef % 2);
      return;
    }
    int mid = (lef + rig) / 2;
    Build(vec, 2 * ind + 1, lef, mid);
    Build(vec, 2 * ind + 2, mid, rig);
    tree[ind] = tree[2 * ind + 1] + tree[2 * ind + 2];
  }
};

int main() {
  std::cin.tie(0);
  std::cout.tie(0);
  std::ios_base::sync_with_stdio(false);
  int number;
  std::cin >> number;
  std::vector<int> vec(number);
  for (int i = 0; i < number; ++i) {
    std::cin >> vec[i];
  }
  Segtree tree;
  tree.Init(number);
  tree.Build(vec, 0, 0, number);
  int requests;
  int operation;
  int first;
  int second;
  std::cin >> requests;
  for (int i = 0; i < requests; ++i) {
    std::cin >> operation >> first >> second;
    --first;
    if (operation == 0) {
      tree.Upgrade(first, second, 0, 0, number);
      continue;
    }
    std::cout << tree.Get(0, 0, number, first, second) *
                     (int)std::pow(-1, first % 2)
              << '\n';
  }
  return 0;
}