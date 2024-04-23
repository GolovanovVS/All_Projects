#include <bits/stdc++.h>

std::mt19937 rnd;
std::vector<std::pair<long long, long long>> QuickSort(
    std::vector<std::pair<long long, long long>>& mas) {
  if ((int)mas.size() <= 1) {
    return mas;
  }
  if ((int)mas.size() == 2) {
    if (mas[0] <= mas[1]) {
      return mas;
    }
    std::vector<std::pair<long long, long long>> read(2);
    read[0] = mas[1];
    read[1] = mas[0];
    return read;
  }
  long long middle = rnd() % (int)mas.size();
  long long border = mas[middle].first;
  std::vector<std::pair<long long, long long>> middle_less;
  std::vector<std::pair<long long, long long>> middle_equal;
  std::vector<std::pair<long long, long long>> middle_more;
  for (auto& element : mas) {
    if (element.first < border) {
      middle_less.push_back(element);
      continue;
    }
    if (element.first > border) {
      middle_more.push_back(element);
      continue;
    }
    middle_equal.push_back(element);
  }
  std::vector<std::pair<long long, long long>> middle_less_sorted =
      QuickSort(middle_less);
  std::vector<std::pair<long long, long long>> middle_more_sorted =
      QuickSort(middle_more);
  std::vector<std::pair<long long, long long>> mas_sorted;
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
  long long number;
  std::cin >> number;
  std::vector<long long> weight(number);
  long long sum = 0;
  long long answer = 0;
  for (long long i = 0; i < number; ++i) {
    std::cin >> weight[i];
    sum += weight[i];
  }
  std::vector<std::pair<long long, long long>> sum_half;
  long long current_sum = 0;
  auto half = (long long)((number + 1) / 2);
  for (long long i = 0; i < half; ++i) {
    current_sum += weight[i];
  }
  std::pair<long long, long long> read{current_sum, 0};
  sum_half.push_back(read);
  for (long long i = half; i < half + number - 1; ++i) {
    current_sum -= weight[i - half];
    current_sum += weight[i % number];
    read = {current_sum, i - half + 1};
    sum_half.push_back(read);
  }
  std::vector<std::pair<long long, long long>> sum_half_sorted =
      QuickSort(sum_half);
  std::pair<long long, long long> borders1{0, half - 1};
  std::pair<long long, long long> borders2{half, number - 1};
  long long index;
  for (auto& element : sum_half_sorted) {
    index = element.second;
    if (index + half <= number) {
      borders1 = {borders1.first, std::min(borders1.second, index - 1)};
      borders2 = {std::max(borders2.first, index + half), borders2.second};
    } else {
      borders1 = {std::max(borders1.first, index + half - number),
                  borders1.second};
      borders2 = {borders2.first, std::min(borders2.second, index - 1)};
    }
    if ((borders1.first > borders1.second) and
        (borders2.first > borders2.second)) {
      answer = element.first;
      break;
    }
  }
  std::cout << answer << ' ' << sum - answer;
  return 0;
}
