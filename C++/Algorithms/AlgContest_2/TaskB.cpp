#include <iostream>
#include <stack>
#include <vector>

int MinimalUp(std::stack<std::pair<int, int>>& min_weight,
              std::pair<int, int> number) {
  while (!min_weight.empty() and min_weight.top().first >= number.first) {
    min_weight.pop();
  }
  if (min_weight.empty()) {
    min_weight.push(number);
    return number.second + 1;
  }
  int index = min_weight.top().second;
  min_weight.push(number);
  return number.second - index;
}

int MinimalDown(std::stack<std::pair<int, int>>& min_weight,
                std::pair<int, int> number, int size) {
  while (!min_weight.empty() and min_weight.top().first >= number.first) {
    min_weight.pop();
  }
  if (min_weight.empty()) {
    min_weight.push(number);
    return size - number.second - 1;
  }
  int index = min_weight.top().second;
  min_weight.push(number);
  return index - number.second - 1;
}

int main() {
  int number;
  int read;
  std::cin >> number;
  std::vector<std::pair<int, int>> weight;
  for (int index = 0; index < number; ++index) {
    std::cin >> read;
    std::pair<int, int> digit{read, index};
    weight.push_back(digit);
  }
  std::vector<int> left_min(number);
  std::stack<std::pair<int, int>> min_weight_left;
  for (int index = 0; index < number; ++index) {
    left_min[index] = MinimalUp(min_weight_left, weight[index]);
  }
  std::vector<int> right_min(number);
  std::stack<std::pair<int, int>> min_weight_right;
  for (int index = number - 1; index >= 0; --index) {
    right_min[index] = MinimalDown(min_weight_right, weight[index], number);
  }
  long long max = 0;
  for (int i = 0; i < number; ++i) {
    max = std::max(max,
                   (long long)weight[i].first * (left_min[i] + right_min[i]));
  }
  std::cout << max;
  return 0;
}
