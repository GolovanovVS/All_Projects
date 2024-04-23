#include <iostream>
#include <vector>
#define ll long long

ll amount = 0;
std::vector<int> timely(2);

std::vector<int> VectorSort(std::vector<int> mas) {
  if ((int)mas.size() <= 1) {
    return mas;
  }
  if ((int)mas.size() == 2) {
    if (mas[0] <= mas[1]) {
      return mas;
    }
    amount++;
    timely = {mas[1], mas[0]};
    return timely;
  }
  int middle = (int)(mas.size() / 2) - 1;
  std::vector<int> mas_less_old = {};
  std::vector<int> mas_more_old = {};
  for (int i = 0; i <= middle; ++i) {
    mas_less_old.push_back(mas[i]);
  }
  for (int i = middle + 1; i < (int)mas.size(); ++i) {
    mas_more_old.push_back(mas[i]);
  }
  std::vector<int> mas_less = VectorSort(mas_less_old);
  std::vector<int> mas_more = VectorSort(mas_more_old);
  std::vector<int> new_mas = {};
  int indicator_less = 0;
  int indicator_more = 0;
  while (indicator_less < (int)mas_less.size() and
      indicator_more < (int)mas_more.size()) {
    if (mas_less[indicator_less] <= mas_more[indicator_more]) {
      new_mas.push_back(mas_less[indicator_less]);
      indicator_less++;
    } else {
      new_mas.push_back(mas_more[indicator_more]);
      indicator_more++;
      amount += (middle - indicator_less + 1);
    }
  }
  if (indicator_less == (int)mas_less.size()) {
    for (int i = indicator_more; i < (int)mas_more.size(); ++i) {
      new_mas.push_back(mas_more[i]);
    }
  }
  if (indicator_more == (int)mas_more.size()) {
    for (int i = indicator_less; i < (int)mas_less.size(); ++i) {
      new_mas.push_back(mas_less[i]);
    }
  }
  return new_mas;
}

int main() {
  int number;
  std::cin >> number;
  std::vector<int> mas(number);
  for (int i = 0; i < number; ++i) {
    std::cin >> mas[i];
  }
  std::vector<int> result = VectorSort(mas);
  std::cout << amount;
}
