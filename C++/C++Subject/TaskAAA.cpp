#include <iostream>

void composition(int** numbers, const int* sizes, int amount, int64_t& general_sum, int current_value, const int depth,
                 bool* used) {
  if (depth == amount) {
    general_sum += current_value;
    return;
  }
  for (int i = 0; i < sizes[depth]; ++i) {
    if (used[i]) {
      continue;
    }
    used[i] = true;
    composition(numbers, sizes, amount, general_sum, current_value * numbers[depth][i], depth + 1, used);
    used[i] = false;
  }
}

int main(int amount, char* size[]) {
  int max_size = 0;
  --amount;
  int** numbers = new int*[amount];
  int* sizes = new int[amount];
  for (int i = 0; i < amount; ++i) {
    sizes[i] = std::atoi(size[i + 1]);
    if (max_size < sizes[i]) {
      max_size = sizes[i];
    }
    numbers[i] = new int[sizes[i]];
    for (int j = 0; j < sizes[i]; ++j) {
      std::cin >> numbers[i][j];
    }
  }
  int64_t general_sum = 0;
  bool* used = new bool[max_size]{};
  composition(numbers, sizes, amount, general_sum, 1, 0, used);
  for (int i = 0; i < amount; ++i) {
    delete[] numbers[i];
  }
  delete[] numbers;
  delete[] used;
  delete[] sizes;
  std::cout << general_sum;
  return 0;
}
