#include <iostream>
#include <random>
#include <vector>

int FindNumber(const std::vector<int>& results, size_t index) {
  std::mt19937 rnd;
  int border = results[rnd() % static_cast<int>(results.size())];
  std::vector<int> border_less;
  std::vector<int> border_equal;
  std::vector<int> border_more;
  for (const auto& element : results) {
    if (element < border) {
      border_less.push_back(element);
      continue;
    }
    if (element > border) {
      border_more.push_back(element);
      continue;
    }
    border_equal.push_back(element);
  }
  if (border_less.size() >= index) {
    return FindNumber(border_less, index);
  }
  if (border_less.size() + border_equal.size() >= index) {
    return border;
  }
  return FindNumber(border_more,
                    index - (border_less.size() + border_equal.size()));
}

int main() {
  const int kLast = 123;
  const int kPreLast = 45;
  const int kModule = 1e7 + 4321;
  int number;
  size_t index;
  std::cin >> number >> index;
  std::vector<int> results(number);
  std::cin >> results[0] >> results[1];
  for (int i = 2; i < number; ++i) {
    results[i] = (results[i - 1] * kLast + results[i - 2] * kPreLast) % kModule;
  }
  std::cout << FindNumber(results, index);
  return 0;
}
