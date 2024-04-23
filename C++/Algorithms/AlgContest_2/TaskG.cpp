#include <cmath>
#include <iostream>
#include <vector>

int Father(int son) { return (son - 1) / 2; }
int YoungSon(int father) { return 2 * father + 1; }
int OldSon(int father) { return 2 * father + 2; }

void SizeLessAmount(std::vector<int>& numbers, int value) {
  numbers.push_back(value);
  int index = (int)numbers.size() - 1;
  while (index != 0 and numbers[Father(index)] < numbers[index]) {
    int time = numbers[Father(index)];
    numbers[Father(index)] = numbers[index];
    numbers[index] = time;
    index = Father(index);
  }
}

void SizeEqualAmount(std::vector<int>& numbers, int value, int amount) {
  if (numbers[0] <= value) {
    return;
  }
  numbers[0] = value;
  int current_index = 0;
  int smallest = 0;
  if (YoungSon(current_index) < amount and
      numbers[YoungSon(current_index)] > numbers[smallest]) {
    smallest = YoungSon(current_index);
  }
  if (OldSon(current_index) < amount and
      numbers[OldSon(current_index)] > numbers[smallest]) {
    smallest = OldSon(current_index);
  }
  while (current_index != smallest) {
    int time = numbers[current_index];
    numbers[current_index] = numbers[smallest];
    numbers[smallest] = time;
    current_index = smallest;
    if (YoungSon(current_index) < amount and
        numbers[YoungSon(current_index)] > numbers[smallest]) {
      smallest = YoungSon(current_index);
    }
    if (OldSon(current_index) < amount and
        numbers[OldSon(current_index)] > numbers[smallest]) {
      smallest = OldSon(current_index);
    }
  }
}

void AddElement(std::vector<int>& numbers, int value, int amount) {
  if ((int)numbers.size() < amount) {
    SizeLessAmount(numbers, value);
    return;
  }
  if ((int)numbers.size() == amount) {
    SizeEqualAmount(numbers, value, amount);
    return;
  }
}

std::vector<int> SortedHeap(std::vector<int>& numbers) {
  if ((int)numbers.size() < 2) {
    return numbers;
  }
  if ((int)numbers.size() == 2) {
    std::vector<int> numbers_sorted{numbers[1], numbers[0]};
    return numbers_sorted;
  }
  std::vector<int> young{numbers[1]};
  std::vector<int> old;
  for (int i = 2; i < (int)numbers.size(); ++i) {
    int step = (int)std::pow(2, (int)std::log2(i));
    if (step + step / 2 - 1 <= i and i <= 2 * step - 2) {
      old.push_back(numbers[i]);
    } else {
      young.push_back(numbers[i]);
    }
  }
  std::vector<int> young_sorted = SortedHeap(young);
  std::vector<int> old_sorted = SortedHeap(old);
  std::vector<int> numbers_sorted;
  int young_pointer = 0;
  int old_pointer = 0;
  while (young_pointer != (int)young_sorted.size() and
      old_pointer != (int)old_sorted.size()) {
    if (young_sorted[young_pointer] < old_sorted[old_pointer]) {
      numbers_sorted.push_back(young_sorted[young_pointer]);
      young_pointer += 1;
    } else {
      numbers_sorted.push_back(old_sorted[old_pointer]);
      old_pointer += 1;
    }
  }
  if (young_pointer == (int)young_sorted.size()) {
    for (int i = old_pointer; i < (int)old_sorted.size(); ++i) {
      numbers_sorted.push_back(old_sorted[i]);
    }
    numbers_sorted.push_back(numbers[0]);
    return numbers_sorted;
  }
  for (int i = young_pointer; i < (int)young_sorted.size(); ++i) {
    numbers_sorted.push_back(young_sorted[i]);
  }
  numbers_sorted.push_back(numbers[0]);
  return numbers_sorted;
}

int main() {
  int number;
  int amount;
  int zero_element;
  int ratio_1;
  int ratio_2;
  int current;
  const int kModule = (int)std::pow(2, 30);
  std::cin >> number >> amount;
  std::cin >> zero_element >> ratio_1 >> ratio_2;
  std::vector<int> numbers;
  int last = zero_element;
  for (int i = 0; i < number; ++i) {
    long long val1 = ((long long)ratio_1 * (long long)last) % kModule;
    long long val2 = ratio_2 % kModule;
    long long value = (val1 + val2) % kModule;
    current = (int)value;
    AddElement(numbers, current, amount);
    last = current;
  }
  std::vector<int> numbers_sorted = SortedHeap(numbers);
  for (int element : numbers_sorted) {
    std::cout << element << ' ';
  }
  return 0;
}
