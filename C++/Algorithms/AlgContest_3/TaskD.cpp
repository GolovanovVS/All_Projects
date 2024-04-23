#include <bits/stdc++.h>
struct Segtree {
  std::vector<int64_t> tree;
  int size;
  void Init(int num) {
    size = 1;
    while (size < num) {
      size *= 2;
    }
    tree.resize(4 * size);
  }
  int64_t Get(int ind, int lef, int rig, int cur) {
    if (lef >= ind) {
      return tree[cur];
    }
    if (rig <= ind) {
      return 0;
    }
    int med = (lef + rig) / 2;
    return Get(ind, lef, med, 2 * cur + 1) + Get(ind, med, rig, 2 * cur + 2);
  }
  void Upgrade(int ind, int cur, int lef, int rig) {
    if (rig - lef == 1) {
      ++tree[cur];
      return;
    }
    int med = (lef + rig) / 2;
    if (ind >= med) {
      Upgrade(ind, 2 * cur + 2, med, rig);
    } else {
      Upgrade(ind, 2 * cur + 1, lef, med);
    }
    tree[cur] = tree[2 * cur + 1] + tree[2 * cur + 2];
  }
};
bool Mor(std::pair<int, int> pa1, std::pair<int, int> pa2) {
  if (pa1.first > pa2.first) {
    return true;
  }
  if (pa1.first == pa2.first) {
    return (pa1.second > pa2.second);
  }
  return false;
}
int main() {
  int num;
  int64_t fin = 0;
  std::cin >> num;
  Segtree tree;
  std::map<std::pair<int, int>, int64_t> map;
  std::set<int> set;
  std::vector<std::pair<int, int>> vec(num);
  std::unordered_map<int, int> ren;
  tree.Init(num);
  int left;
  int right;
  for (int i = 0; i < num; ++i) {
    std::cin >> left >> right;
    set.insert(-left);
    set.insert(right);
    vec[i].second = -left;
    vec[i].first = right;
  }
  int cnt = 0;
  for (auto it : set) {
    ren[it] = ++cnt;
  }
  std::sort(vec.begin(), vec.end(), Mor);
  for (int i = 0; i < num; ++i) {
    right = ren[vec[i].first];
    left = ren[vec[i].second];
    if (map.find({right, left}) != map.end()) {
      fin += map[{right, left}];
    } else {
      int64_t res = tree.Get(left, 0, tree.size, 0);
      fin += res;
      map[{right, left}] = res;
    }
    tree.Upgrade(left, 0, 0, tree.size);
  }
  std::cout << fin;
}