#include <iostream>
#include <queue>
#include <string>
#include <vector>

class MinAndMax {
 private:
  std::priority_queue<int> max_heap_;
  std::priority_queue<int> max_heap_trash_;
  std::priority_queue<int, std::vector<int>, std::greater<>> min_heap_;
  std::priority_queue<int, std::vector<int>, std::greater<>> min_heap_trash_;

 public:
  std::string Insert(int number) {
    max_heap_.push(number);
    min_heap_.push(number);
    return "ok";
  }

  std::string Extract(bool min) {
    while (!(min ? min_heap_trash_ : max_heap_trash_).empty() and
           min_heap_.top() == min_heap_trash_.top()) {
      min_heap_.pop();
      min_heap_trash_.pop();
    }
    if (min_heap_.empty()) {
      return "error";
    }
    int element = min_heap_.top();
    min_heap_trash_.push(element);
    max_heap_trash_.push(element);
    while (!min_heap_trash_.empty() and
           min_heap_.top() == min_heap_trash_.top()) {
      min_heap_.pop();
      min_heap_trash_.pop();
    }
    return std::to_string(element);
  }

  std::string ExtractMin() {
    while (!min_heap_trash_.empty() and
           min_heap_.top() == min_heap_trash_.top()) {
      min_heap_.pop();
      min_heap_trash_.pop();
    }
    if (min_heap_.empty()) {
      return "error";
    }
    int element = min_heap_.top();
    min_heap_trash_.push(element);
    max_heap_trash_.push(element);
    while (!min_heap_trash_.empty() and
           min_heap_.top() == min_heap_trash_.top()) {
      min_heap_.pop();
      min_heap_trash_.pop();
    }
    return std::to_string(element);
  }

  std::string ExtractMax() {
    while (!max_heap_trash_.empty() and
           max_heap_.top() == max_heap_trash_.top()) {
      max_heap_.pop();
      max_heap_trash_.pop();
    }
    if (max_heap_.empty()) {
      return "error";
    }
    int element = max_heap_.top();
    max_heap_trash_.push(element);
    min_heap_trash_.push(element);
    while (!max_heap_trash_.empty() and
           max_heap_.top() == max_heap_trash_.top()) {
      max_heap_.pop();
      max_heap_trash_.pop();
    }
    return std::to_string(element);
  }

  std::string GetMin() {
    while (!min_heap_trash_.empty() and
           min_heap_.top() == min_heap_trash_.top()) {
      min_heap_.pop();
      min_heap_trash_.pop();
    }
    if (min_heap_.empty()) {
      return "error";
    }
    return std::to_string(min_heap_.top());
  }

  std::string GetMax() {
    while (!max_heap_trash_.empty() and
           max_heap_.top() == max_heap_trash_.top()) {
      max_heap_.pop();
      max_heap_trash_.pop();
    }
    if (max_heap_.empty()) {
      return "error";
    }
    return std::to_string(max_heap_.top());
  }

  int Size() {
    return static_cast<int>(max_heap_.size() - max_heap_trash_.size());
  }

  std::string Clear() {
    while (!max_heap_.empty()) {
      max_heap_.pop();
    }
    while (!max_heap_trash_.empty()) {
      max_heap_trash_.pop();
    }
    while (!min_heap_.empty()) {
      min_heap_.pop();
    }
    while (!min_heap_trash_.empty()) {
      min_heap_trash_.pop();
    }
    return "ok";
  }
};

int main() {
  MinAndMax min_and_max;
  int number;
  std::string operation;
  std::cin >> number;
  for (int i = 0; i < number; ++i) {
    std::cin >> operation;
    if (operation == "insert") {
      int value;
      std::cin >> value;
      std::cout << min_and_max.Insert(value) << '\n';
    }
    if (operation == "extract_min") {
      std::cout << min_and_max.ExtractMin() << '\n';
    }
    if (operation == "get_min") {
      std::cout << min_and_max.GetMin() << '\n';
    }
    if (operation == "extract_max") {
      std::cout << min_and_max.ExtractMax() << '\n';
    }
    if (operation == "get_max") {
      std::cout << min_and_max.GetMax() << '\n';
    }
    if (operation == "size") {
      std::cout << min_and_max.Size() << '\n';
    }
    if (operation == "clear") {
      std::cout << min_and_max.Clear() << '\n';
    }
  }
  return 0;
}