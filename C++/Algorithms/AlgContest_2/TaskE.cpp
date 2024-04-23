#include <iostream>
#include <vector>

struct Details {
  int number;
  int current_amount;
  int index;
};

std::vector<Details> timely(2);

std::vector<Details> VectorSort(std::vector<Details>& mas) {
  if ((int)mas.size() <= 1) {
    return mas;
  }
  if ((int)mas.size() == 2) {
    if (mas[0].number <= mas[1].number) {
      return mas;
    }
    mas[0].current_amount++;
    timely = {mas[1], mas[0]};
    return timely;
  }
  int middle = (int)(mas.size() / 2) - 1;
  std::vector<Details> mas_less_old = {};
  std::vector<Details> mas_more_old = {};
  for (int i = 0; i <= middle; ++i) {
    mas_less_old.push_back(mas[i]);
  }
  for (int i = middle + 1; i < (int)mas.size(); ++i) {
    mas_more_old.push_back(mas[i]);
  }
  std::vector<Details> mas_less = VectorSort(mas_less_old);
  std::vector<Details> mas_more = VectorSort(mas_more_old);
  std::vector<Details> new_mas = {};
  int current_difference = 0;
  int indicator_less = 0;
  int indicator_more = 0;
  while (indicator_less < (int)mas_less.size() and
      indicator_more < (int)mas_more.size()) {
    if (mas_less[indicator_less].number <= mas_more[indicator_more].number) {
      mas_less[indicator_less].current_amount += current_difference;
      new_mas.push_back(mas_less[indicator_less]);
      indicator_less++;
    } else {
      new_mas.push_back(mas_more[indicator_more]);
      indicator_more++;
      current_difference++;
    }
  }
  if (indicator_less == (int)mas_less.size()) {
    for (int i = indicator_more; i < (int)mas_more.size(); ++i) {
      new_mas.push_back(mas_more[i]);
    }
  }
  if (indicator_more == (int)mas_more.size()) {
    for (int i = indicator_less; i < (int)mas_less.size(); ++i) {
      mas_less[i].current_amount += current_difference;
      new_mas.push_back(mas_less[i]);
    }
  }
  return new_mas;
}

int main() {
  int number;
  int read;
  std::cin >> number;
  std::vector<Details> mas;
  for (int i = 0; i < number; ++i) {
    std::cin >> read;
    Details digit{read, 0, i};
    mas.push_back(digit);
  }
  std::vector<Details> new_mas = VectorSort(mas);
  std::vector<int> result(number);
  for (auto& element : new_mas) {
    result[element.index] = element.current_amount;
  }
  for (auto& element : result) {
    std::cout << element << ' ';
  }
}
