#include <cmath>
#include <iostream>
#include <vector>

struct Segtree {
  struct Node {
    int sum = 0;
  };
  std::vector<Node> tree;
  int size = 1;
  void Init(int number) {
    while (size < number) {
      size *= 2;
    }
    tree.resize(2 * size - 1);
  }
  void Build(std::vector<int>& vector, int index, int left, int right) {
    if (left >= (int)vector.size()) {
      return;
    }
    if (right - left == 1) {
      tree[index].sum = vector[left];
      return;
    }
    int middle = (right + left) / 2;
    Build(vector, 2 * index + 1, left, middle);
    Build(vector, 2 * index + 2, middle, right);
    tree[index].sum =
        std::max(tree[2 * index + 1].sum, tree[2 * index + 2].sum);
  }
  void Build(std::vector<int>& vector) {
    Init((int)vector.size());
    Build(vector, 0, 0, size);
  }
  void Upgrade(int index, int change, int left, int right, int current_index) {
    if (right - left == 1) {
      tree[current_index].sum = change;
      return;
    }
    int med = (left + right) / 2;
    if (index < med) {
      Upgrade(index, change, left, med, 2 * current_index + 1);
    } else {
      Upgrade(index, change, med, right, 2 * current_index + 2);
    }
    tree[current_index].sum = std::max(tree[2 * current_index + 1].sum,
                                       tree[2 * current_index + 2].sum);
  }
  int Get(int ind, int val, int index, int left, int right) {
    if (tree[index].sum < val or right <= ind) {
      return -1;
    }
    if (right - left == 1) {
      return left;
    }
    int med = (left + right) / 2;
    if (left >= ind) {
      if (tree[2 * index + 1].sum >= val) {
        return Get(ind, val, 2 * index + 1, left, med);
      }
      return Get(ind, val, 2 * index + 2, med, right);
    }
    int result1 = Get(ind, val, 2 * index + 1, left, med);
    int result2 = Get(ind, val, 2 * index + 2, med, right);
    if (result1 != -1) {
      return result1;
    }
    return result2;
  }
};

int main() {
  int number;
  int request;
  std::vector<int> result;
  std::cin >> number >> request;
  std::vector<int> vector(number);
  for (int i = 0; i < number; ++i) {
    std::cin >> vector[i];
  }
  Segtree tree;
  tree.Build(vector);
  int operation;
  for (int i = 0; i < request; ++i) {
    int ind;
    int val;
    std::cin >> operation >> ind >> val;
    --ind;
    if (operation == 1) {
      int answer = tree.Get(ind, val, 0, 0, tree.size);
      if (answer == -1) {
        result.push_back(answer);
      } else {
        result.push_back(answer + 1);
      }
      continue;
    }
    tree.Upgrade(ind, val, 0, tree.size, 0);
  }
  for (auto& element : result) {
    std::cout << element << '\n';
  }
  return 0;
}