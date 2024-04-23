#include <bits/stdc++.h>

struct Node {
  public:
  int key = -1;
  int value = -1;
  Node* left = nullptr;
  Node* right = nullptr;
  Node(int key, int priority) : key(key), value(priority) {}
};

Node* Build(std::vector<int>& vector) {
  std::stack<Node*> stack;
  for (int i = 0; i < (int)vector.size(); ++i) {
    Node* last = nullptr;
    Node* current_node = new Node(i, vector[i]);
    while (!stack.empty() and (stack.top())->value > current_node->value) {
      last = stack.top();
      stack.pop();
    }
    if (!stack.empty()) {
      (stack.top())->right = current_node;
    }
    current_node->left = last;
    stack.push(current_node);
  }
  while (stack.size() != 1) {
    stack.pop();
  }
  return stack.top();
}

void DFS(Node* root, std::vector<std::vector<int>>& answer) {
  if (root == nullptr) {
    return;
  }
  if (root->right != nullptr) {
    answer[root->right->key][0] = root->key;
    answer[root->key][2] = root->right->key;
    DFS(root->right, answer);
  }
  if (root->left != nullptr) {
    answer[root->left->key][0] = root->key;
    answer[root->key][1] = root->left->key;
    DFS(root->left, answer);
  }
}

void Solve(int num, std::vector<int>& priorities) {
  std::vector<std::vector<int>> ans(num, std::vector<int>(3, -1));
  DFS(Build(priorities), ans);
  for (auto& vector : ans) {
    std::cout << vector[0] << ' ' << vector[1] << ' ' << vector[2] << '\n';
  }
}

int main() {
  int number;
  int key;
  std::cin >> number;
  std::vector<int> priorities(number);
  for (int i = 0; i < number; ++i) {
    std::cin >> key >> priorities[i];
  }
  std::cout << "YES" << '\n';
  Solve(number, priorities);
}