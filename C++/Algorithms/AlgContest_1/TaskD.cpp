#include <cmath>
#include <iostream>
#include <vector>

std::vector<int> QuickSort(std::vector<int>& mas) {
  std::vector<int> read(2);
  if ((int)mas.size() <= 1) {
    return mas;
  }
  if ((int)mas.size() == 2) {
    if (mas[0] <= mas[1]) {
      return mas;
    }
    read[0] = mas[1];
    read[1] = mas[0];
    return read;
  }
  unsigned int global_time = time(nullptr);
  srand(global_time - 1);
  int middle = rand() % (int)mas.size();
  int border = mas[middle];
  std::vector<int> middle_less = {};
  std::vector<int> middle_equal = {};
  std::vector<int> middle_more = {};
  for (auto& element : mas) {
    if (element < border) {
      middle_less.push_back(element);
    } else if (element > border) {
      middle_more.push_back(element);
    } else {
      middle_equal.push_back(element);
    }
  }
  std::vector<int> middle_less_sorted = QuickSort(middle_less);
  std::vector<int> middle_more_sorted = QuickSort(middle_more);
  std::vector<int> mas_sorted = {};
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

bool LengthIsOkay(std::vector<int>& mas, int length, int amount) {
  int size = (int)mas.size();
  int current_coordinate = mas[0];
  int current_index = 0;
  for (int i = 0; i < amount; ++i) {
    while ((current_coordinate + length) >= mas[current_index]) {
      current_index += 1;
      if (current_index == size) {
        return true;
      }
    }
    current_coordinate = mas[current_index];
  }
  return false;
}

int BinSearch(std::vector<int>& mas, int left, int right, int amount) {
  if (right - left == 0) {
    return 0;
  }
  int middle;
  while (1 < (right - left)) {
    middle = left + (right - left) / 2;
    if (LengthIsOkay(mas, middle, amount)) {
      right = middle;
    } else {
      left = middle;
    }
  }
  return right;
}

int main() {
  int number;
  int amount;
  std::cin >> number >> amount;
  std::vector<int> mas(number);
  for (int i = 0; i < number; ++i) {
    std::cin >> mas[i];
  }
  std::vector<int> mas_sorted = QuickSort(mas);
  std::cout << BinSearch(mas_sorted, -1, mas_sorted[number - 1] - mas_sorted[0],
                         amount);
  return 0;
}
