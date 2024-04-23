#include <bits/stdc++.h>

std::mt19937 rnd;
std::vector<int> QuickSort(std::vector<int>& mas) {
  std::vector<int> read(2);
  if (mas.size() <= 1) {
    return mas;
  }
  if (mas.size() == 2) {
    if (mas[0] <= mas[1]) {
      return mas;
    }
    read[0] = mas[1];
    read[1] = mas[0];
    return read;
  }
  int middle = rnd() % mas.size();
  int border = mas[middle];
  std::vector<int> middle_less;
  std::vector<int> middle_equal;
  std::vector<int> middle_more;
  for (auto& element : mas) {
    if (element < border) {
      middle_less.push_back(element);
      continue;
    }
    if (element > border) {
      middle_more.push_back(element);
      continue;
    }
    middle_equal.push_back(element);
  }
  std::vector<int> middle_less_sorted = QuickSort(middle_less);
  std::vector<int> middle_more_sorted = QuickSort(middle_more);
  std::vector<int> mas_sorted;
  for (auto& number : middle_less_sorted) {
    mas_sorted.push_back(number);
  }
  for (auto& number : middle_equal) {
    mas_sorted.push_back(number);
  }
  for (auto& number : middle_more_sorted) {
    mas_sorted.push_back(number);
  }
  return mas_sorted;
}

int main() {
  int number;
  std::cin >> number;
  std::vector<int> mas(number);
  for (int i = 0; i < number; ++i) {
    std::cin >> mas[i];
  }
  std::vector<int> mas_sorted = QuickSort(mas);
  for (auto& element : mas_sorted) {
    std::cout << element << ' ';
  }
  return 0;
}
