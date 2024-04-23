#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

std::vector<int> FindMinLeft(std::vector<int> numbers) {
  std::vector<int> min_left((int)numbers.size());
  int min = INT32_MAX;
  for (int i = 0; i < (int)numbers.size(); ++i) {
    if (min >= numbers[i]) {
      min = numbers[i];
    }
    min_left[i] = min;
  }
  return min_left;
}

std::vector<int> FindMinRight(std::vector<int> numbers) {
  std::vector<int> min_right((int)numbers.size());
  int min = INT32_MAX;
  for (int i = (int)numbers.size() - 1; i > -1; --i) {
    if (min >= numbers[i]) {
      min = numbers[i];
    }
    min_right[i] = min;
  }
  return min_right;
}

int main() {
  int number;
  int requests;
  int left_border;
  int right_border;
  std::cin >> number;
  std::vector<int> numbers(number);
  for (int i = 0; i < number; ++i) {
    std::cin >> numbers[i];
  }
  std::vector<int> min_left = FindMinLeft(numbers);
  std::vector<int> min_right = FindMinRight(numbers);
  std::cin >> requests;
  for (int i = 0; i < requests; ++i) {
    std::cin >> left_border >> right_border;
    if (min_left[left_border - 1] > min_right[right_border - 1]) {
      std::cout << min_right[right_border - 1] << '\n';
    } else {
      std::cout << min_left[left_border - 1] << '\n';
    }
  }
  return 0;
}