#include <iostream>
#include <vector>

std::vector<std::pair<int, int>> timely(2);

std::vector<std::pair<int, int>> VectorSort(
    std::vector<std::pair<int, int>> mas) {
  if ((int)mas.size() <= 1) {
    return mas;
  }
  if ((int)mas.size() == 2) {
    if (mas[0].first <= mas[1].first) {
      return mas;
    }
    timely = {mas[1], mas[0]};
    return timely;
  }
  int middle = (int)(mas.size() / 2) - 1;
  std::vector<std::pair<int, int>> mas_less_old = {};
  std::vector<std::pair<int, int>> mas_more_old = {};
  for (int i = 0; i <= middle; ++i) {
    mas_less_old.push_back(mas[i]);
  }
  for (int i = middle + 1; i < (int)mas.size(); ++i) {
    mas_more_old.push_back(mas[i]);
  }
  std::vector<std::pair<int, int>> mas_less = VectorSort(mas_less_old);
  std::vector<std::pair<int, int>> mas_more = VectorSort(mas_more_old);
  std::vector<std::pair<int, int>> new_mas = {};
  int indicator_less = 0;
  int indicator_more = 0;
  while (indicator_less < (int)mas_less.size() and
      indicator_more < (int)mas_more.size()) {
    if (mas_less[indicator_less].first <= mas_more[indicator_more].first) {
      new_mas.push_back(mas_less[indicator_less]);
      indicator_less++;
    } else {
      new_mas.push_back(mas_more[indicator_more]);
      indicator_more++;
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
  int begin;
  int end;
  int number;
  int read_0;
  int read_1;
  std::cin >> number;
  std::vector<std::pair<int, int>> mas(number);
  for (int i = 0; i < number; ++i) {
    std::cin >> read_0 >> read_1;
    std::pair<int, int> read = {read_0, read_1};
    mas[i] = read;
  }
  std::vector<std::pair<int, int>> mas_sorted(number);
  mas_sorted = VectorSort(mas);
  std::vector<std::pair<int, int>> intervals;
  begin = mas_sorted[0].first;
  end = mas_sorted[0].second;
  for (int i = 0; i < number; ++i) {
    if (end >= mas_sorted[i].first) {
      end = std::max(mas_sorted[i].second, end);
    } else {
      std::pair<int, int> read = {begin, end};
      intervals.push_back(read);
      begin = mas_sorted[i].first;
      end = mas_sorted[i].second;
    }
  }
  std::pair<int, int> read = {begin, end};
  intervals.push_back(read);
  std::cout << (int)intervals.size() << '\n';
  for (auto& interval : intervals) {
    std::cout << interval.first << ' ' << interval.second << '\n';
  }
  return 0;
}
