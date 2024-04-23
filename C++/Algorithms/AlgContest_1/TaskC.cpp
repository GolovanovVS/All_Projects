#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

int FindMinAmongMax(const std::vector<int>& mas_from_a,
                    const std::vector<int>& mas_from_b) {
  int min = INT32_MAX;
  int index_min = -1;
  for (int i = 0; i < (int)mas_from_a.size(); ++i) {
    if (std::max(mas_from_a[i], mas_from_b[i]) < min) {
      min = std::max(mas_from_a[i], mas_from_b[i]);
      index_min = i;
    }
  }
  return index_min + 1;
}

int main() {
  int amount_a;
  int amount_b;
  int number_a;
  int number_b;
  int read;
  int requests;
  int length;
  std::cin >> amount_a >> amount_b >> length;
  std::vector<std::vector<int>> mas_a(amount_a);
  std::vector<std::vector<int>> mas_b(amount_b);
  for (int i = 0; i < amount_a; ++i) {
    for (int j = 0; j < length; ++j) {
      std::cin >> read;
      mas_a[i].push_back(read);
    }
  }
  for (int i = 0; i < amount_b; ++i) {
    for (int j = 0; j < length; ++j) {
      std::cin >> read;
      mas_b[i].push_back(read);
    }
  }
  std::cin >> requests;
  for (int i = 0; i < requests; ++i) {
    std::cin >> number_a >> number_b;
    std::cout << FindMinAmongMax(mas_a[number_a - 1], mas_b[number_b - 1])
              << '\n';
  }
  return 0;
}