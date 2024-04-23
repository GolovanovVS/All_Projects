#include <bits/stdc++.h>
struct SegmentTree {
  struct Node {
    long long sum = 0;
  };
  std::vector<Node> tree;
  int size;
  void Init(int num) {
    size = 1;
    while (size < num) {
      size *= 2;
    }
    tree.resize(4 * size);
  }
  void Upgrade(int ind, int val, int cur, int lef, int rig) {
    if (rig - lef == 1) {
      tree[cur].sum += val;
      return;
    }
    int med = (lef + rig) / 2;
    if (ind >= med) {
      Upgrade(ind, val, 2 * cur + 2, med, rig);
    } else {
      Upgrade(ind, val, 2 * cur + 1, lef, med);
    }
    tree[cur].sum = tree[2 * cur + 1].sum + tree[2 * cur + 2].sum;
  }
  long long Get(int ind, int cur, int lef, int rig) {
    if (ind < lef) {
      return 0;
    }
    if (rig <= ind + 1) {
      return tree[cur].sum;
    }
    int med = (lef + rig) / 2;
    return Get(ind, 2 * cur + 2, med, rig) + Get(ind, 2 * cur + 1, lef, med);
  }
};

int FindIndex(const std::vector<int>& num, int value, int left, int right) {
  if (left == right) {
    return left;
  }
  if (right - left == 1) {
    if (num[left] == value) {
      return left;
    }
    return right;
  }
  int middle = (left + right) / 2;
  if (num[middle] < value) {
    return FindIndex(num, value, left, middle - 1);
  }
  if (num[middle] > value) {
    return FindIndex(num, value, middle + 1, right);
  }
  return middle;
}

int main() {
  std::cin.tie(0);
  std::cout.tie(0);
  std::ios_base::sync_with_stdio(false);
  int req;
  std::cin >> req;
  std::vector<std::pair<char, int>> vec(req);
  std::set<int> mn;
  std::unordered_map<int, int> cord;
  std::vector<int> fin(req);
  std::vector<long long> result;
  SegmentTree tr;
  for (int i = 0; i < req; ++i) {
    std::cin >> vec[i].first >> vec[i].second;
    mn.insert(vec[i].second);
  }
  int nu = 0;
  for (auto it : mn) {
    cord[it] = nu++;
  }
  tr.Init(nu + 1);
  for (int i = 0; i < req; ++i) {
    if (vec[i].first == '+') {
      tr.Upgrade(cord[vec[i].second], vec[i].second, 0, 0, tr.size);
    } else {
      result.push_back(tr.Get(cord[vec[i].second], 0, 0, tr.size));
    }
  }
  for (auto el : result) {
    std::cout << el << '\n';
  }
  return 0;
}