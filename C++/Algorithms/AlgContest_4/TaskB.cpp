#include <bits/stdc++.h>

struct Node {
 public:
  int value;
  int max_depth = 1;
  Node* left = nullptr;
  Node* right = nullptr;
  Node* dad = nullptr;
  explicit Node(int val) : value(val) {}
};

class AVLTree {
 private:
  std::vector<Node*> to_delete_;
  Node* root_;

  int Delta(Node*& node) {
    if (node == nullptr) {
      return 0;
    }
    if (node->left == nullptr and node->right == nullptr) {
      return 0;
    }
    if (node->left != nullptr and node->right == nullptr) {
      return node->left->max_depth;
    }
    if (node->left == nullptr) {
      return -node->right->max_depth;
    }
    return node->left->max_depth - node->right->max_depth;
  }

  void Depth(Node*& node) {
    if (node == nullptr) {
      return;
    }
    if (node->left == nullptr and node->right == nullptr) {
      node->max_depth = 1;
    }
    if (node->right == nullptr and node->left != nullptr) {
      node->max_depth = node->left->max_depth + 1;
    } else if (node->left == nullptr and node->right != nullptr) {
      node->max_depth = node->right->max_depth + 1;
    } else if (node->left != nullptr) {
      node->max_depth =
          std::max(node->right->max_depth, node->left->max_depth) + 1;
    }
    Depth(node->dad);
  }

  void TurnLeft(Node*& node) {
    if (Delta(node) != 2) {
      throw std::logic_error("Дельта не +2");
    }
    Node* other = node->left;
    if (Delta(node->left) != -1) {
      if (node->dad != nullptr and node->dad->left == node) {
        node->dad->left = other;
      }
      if (node->dad != nullptr and node->dad->right == node) {
        node->dad->right = other;
      }
      other->dad = node->dad;
      if (other->right != nullptr) {
        other->right->dad = node;
      }
      node->left = other->right;
      node->dad = other;
      other->right = node;
      if (root_ == node) {
        root_ = node->dad;
      }
      Depth(node);
    } else {
      TurnRight(node->left);
      TurnLeft(node);
    }
  }
  void TurnRight(Node*& node) {
    if (Delta(node) != -2) {
      throw std::logic_error("Дельта не -2");
    }
    Node* other = node->right;
    if (Delta(node->left) != 1) {
      if (node->dad != nullptr and node->dad->left == node) {
        node->dad->left = other;
      }
      if (node->dad != nullptr and node->dad->right == node) {
        node->dad->right = other;
      }
      other->dad = node->dad;
      if (other->left != nullptr) {
        other->left->dad = node;
      }
      node->right = other->left;
      node->dad = other;
      other->left = node;
      if (root_ == node) {
        root_ = node->dad;
      }
      Depth(node);
    } else {
      TurnLeft(node->right);
      TurnRight(node);
    }
  }

  void CheckTree(Node*& node) {
    Node* current_node = node->dad;
    while (current_node != nullptr) {
      if (current_node->right == nullptr) {
        current_node->max_depth = current_node->left->max_depth + 1;
        if (current_node->max_depth == 3) {
          TurnLeft(current_node);
        }
      } else if (current_node->left == nullptr) {
        current_node->max_depth = current_node->right->max_depth + 1;
        if (current_node->max_depth == 3) {
          TurnRight(current_node);
        }
      } else {
        current_node->max_depth = std::max(current_node->right->max_depth,
                                           current_node->left->max_depth) +
                                  1;
        if (current_node->left->max_depth - current_node->right->max_depth ==
            2) {
          TurnLeft(current_node);
        }
        if (current_node->left->max_depth - current_node->right->max_depth ==
            -2) {
          TurnRight(current_node);
        }
      }
      current_node = current_node->dad;
    }
  }

 public:
  void AddNode(int value) {
    Node* node = new Node(value);
    to_delete_.push_back(node);
    if (to_delete_.size() == 1) {
      root_ = node;
      return;
    }
    Node* current_node = root_;
    while (current_node != nullptr and current_node->value != node->value) {
      if (current_node->value < node->value) {
        if (current_node->right != nullptr) {
          current_node = current_node->right;
          continue;
        }
        current_node->right = node;
        node->dad = current_node;
        continue;
      }
      if (current_node->value > node->value) {
        if (current_node->left != nullptr) {
          current_node = current_node->left;
          continue;
        }
        current_node->left = node;
        node->dad = current_node;
        continue;
      }
      return;
    }
    CheckTree(node);
  }

  int FindNode(int value) {
    if (to_delete_.empty()) {
      return -1;
    }
    Node* current_node = root_;
    int mean = -1;
    bool flag = true;
    while (current_node != nullptr) {
      mean = current_node->value;
      if (mean < value) {
        current_node = current_node->right;
        continue;
      }
      if (mean > value) {
        flag = false;
        current_node = current_node->left;
        continue;
      }
      return mean;
    }
    if (flag) {
      return -1;
    }
    return mean;
  }

  void DeleteNodes() {
    for (auto& node : to_delete_) {
      delete node;
    }
  }
};

int main() {
  int amount;
  const int kPower = 1e9;
  std::cin >> amount;
  char operation;
  int number;
  int current_res = 0;
  AVLTree tree;
  std::vector<Node*> vector;
  for (int i = 0; i < amount; ++i) {
    std::cin >> operation >> number;
    if (operation == '+') {
      int nec = number + current_res;
      nec %= static_cast<int>(kPower);
      tree.AddNode(nec);
      continue;
    }
    current_res = tree.FindNode(number);
    std::cout << current_res << '\n';
  }
  tree.DeleteNodes();
}