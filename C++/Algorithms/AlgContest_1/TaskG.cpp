#include <cmath>
#include <iostream>
#include <vector>

int FindDigit(long long number, int digit) {
  const int kModule = 256;
  number /= (long long)(std::pow(kModule, digit));
  return static_cast<int>(number % kModule);
}

void SortByDigit(std::vector<long long>& array, int digit) {
  const int kModule = 256;
  std::vector<std::vector<long long>> vector(kModule);
  for (auto& item : array) {
    int index = FindDigit(item, digit);
    vector[index].push_back(item);  // распределены по разрядам в 256-ричной СС
  }
  int current_amount = 0;
  for (int i = 0; i < kModule; ++i) {
    for (auto& number : vector[i]) {
      array[current_amount] = number;
      ++current_amount;
    }
  }
}

void LSD(std::vector<long long>& array) {
  const int kBitInByte = 8;
  for (int i = 0; i < kBitInByte; ++i) {
    SortByDigit(array, i);
  }
}

int main() {
  int amount;
  std::cin >> amount;
  std::vector<long long> array(amount);
  for (int i = 0; i < amount; ++i) {
    std::cin >> array[i];
  }
  LSD(array);
  for (auto& element : array) {
    std::cout << element << '\n';
  }
  return 0;
}
