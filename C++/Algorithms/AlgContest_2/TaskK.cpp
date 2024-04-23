#include <deque>
#include <iostream>
#include <queue>
#include <vector>

int AddElementMin(std::deque<std::pair<int, int>>& minimal,
                  std::pair<int, int> number, int size) {
  std::pair<int, int> min = minimal.front();
  if (min.second + size == number.second) {
    minimal.pop_front();
  }
  while (!minimal.empty() and minimal.back().first >= number.first) {
    minimal.pop_back();
  }
  minimal.push_back(number);
  return minimal.front().first;
}

std::vector<std::vector<int>> FindMinInHorizontal(
    const std::vector<std::vector<int>>& matrix, int size) {
  std::vector<std::vector<int>> result;
  for (int i = 0; i < (int)matrix.size(); ++i) {
    std::vector<int> result_i;
    std::queue<std::pair<int, int>> numbers;
    std::deque<std::pair<int, int>> minimal;
    for (int j = 0; j < (int)matrix[0].size(); ++j) {
      std::pair<int, int> number{matrix[i][j], j};
      numbers.push(number);
      AddElementMin(minimal, number, size);
      if (j >= size - 1) {
        numbers.pop();
        result_i.push_back(minimal.front().first);
      }
    }
    result.push_back(result_i);
  }
  return result;
}

std::vector<std::vector<int>> TransposedMatrix(
    std::vector<std::vector<int>>& matrix, int horizontal, int vertical) {
  std::vector<std::vector<int>> result(vertical);
  std::vector<int> empty(horizontal);
  for (int i = 0; i < vertical; ++i) {
    result[i] = empty;
    for (int j = 0; j < horizontal; ++j) {
      result[i][j] = matrix[j][i];
    }
  }
  return result;
}

int main() {
  int number;
  int size;
  std::cin >> number >> size;
  std::vector<std::vector<int>> mas(number);
  for (int i = 0; i < number; ++i) {
    std::vector<int> read(number);
    for (int j = 0; j < number; ++j) {
      std::cin >> read[j];
    }
    mas[i] = read;
  }
  std::vector<std::vector<int>> result = FindMinInHorizontal(mas, size);
  result = TransposedMatrix(result, number, number + 1 - size);
  std::vector<std::vector<int>> answer = FindMinInHorizontal(result, size);
  answer = TransposedMatrix(answer, number + 1 - size, number + 1 - size);
  for (auto& vector : answer) {
    for (int element : vector) {
      std::cout << element << ' ';
    }
    std::cout << '\n';
  }
  return 0;
}
