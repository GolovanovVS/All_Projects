#include <deque>
#include <iostream>
#include <queue>
#include <string>

class Hat {
  private:
  std::queue<std::pair<int, int>> students_;
  std::deque<std::pair<int, int>> minimal_;

  public:
  void Enqueue(int number, int index) {
    std::pair<int, int> pair{number, index};
    students_.push(pair);
    while (!minimal_.empty() and minimal_.back().first >= number) {
      minimal_.pop_back();
    }
    minimal_.push_back(pair);
  }

  int Dequeue() {
    if (students_.empty()) {
      return -1;
    } else {
      if (students_.front().second == minimal_.front().second) {
        minimal_.pop_front();
      }
      int answer = students_.front().first;
      students_.pop();
      return answer;
    }
  }

  int Front() {
    if (students_.empty()) {
      return -1;
    } else {
      return students_.front().first;
    }
  }

  int Size() { return students_.size(); }

  void Clear() {
    while (!students_.empty()) {
      students_.pop();
    }
    while (!minimal_.empty()) {
      minimal_.pop_front();
    }
  }

  int Min() {
    if (students_.empty()) {
      return -1;
    } else {
      return minimal_.front().first;
    }
  }
};

int main() {
  int amount;
  std::string operation;
  Hat hat;
  std::cin >> amount;
  for (int index = 0; index < amount; ++index) {
    std::cin >> operation;
    int answer;
    if (operation == "enqueue") {
      int number;
      std::cin >> number;
      hat.Enqueue(number, index);
      std::cout << "ok" << '\n';
    }
    if (operation == "dequeue") {
      answer = hat.Dequeue();
      if (answer == -1) {
        std::cout << "error" << '\n';
      } else {
        std::cout << answer << '\n';
      }
    }
    if (operation == "front") {
      answer = hat.Front();
      if (answer == -1) {
        std::cout << "error" << '\n';
      } else {
        std::cout << answer << '\n';
      }
    }
    if (operation == "size") {
      std::cout << hat.Size() << '\n';
    }
    if (operation == "clear") {
      hat.Clear();
      std::cout << "ok" << '\n';
    }
    if (operation == "min") {
      answer = hat.Min();
      if (answer == -1) {
        std::cout << "error" << '\n';
      } else {
        std::cout << answer << '\n';
      }
    }
  }
  return 0;
}
