#include <bits/stdc++.h>

int main() {
  std::string line_1;
  std::string line_2;
  std::cin >> line_1 >> line_2;
  std::vector<std::vector<int>> matrix;
  matrix.resize(line_1.length() + 1);
  matrix[0].resize(line_2.size() + 1);
  for (int i = 1; i < int(line_1.size()) + 1; ++i) {
    matrix[i].resize(line_2.size() + 1);
    for (int j = 1; j < int(line_2.size()) + 1; ++j) {
      matrix[i][j] = std::max(matrix[i - 1][j], matrix[i][j - 1]);
      if (line_1[i - 1] == line_2[j - 1]) {
        matrix[i][j] = std::max(matrix[i][j], matrix[i - 1][j - 1] + 1);
      }
    }
  }
  std::cout << matrix[line_1.size()][line_2.size()] << '\n';
  std::vector<int> indexes_1(matrix[line_1.size()][line_2.size()]);
  std::vector<int> indexes_2(matrix[line_1.size()][line_2.size()]);
  int current_index = matrix[line_1.size()][line_2.size()] - 1;
  int ind_1 = int(line_1.size());
  int ind_2 = int(line_2.size());
  while (ind_1 * ind_2 != 0) {
    if ((matrix[ind_1][ind_2] == matrix[ind_1 - 1][ind_2 - 1] + 1) and
        (line_1[ind_1 - 1] == line_2[ind_2 - 1])) {
      indexes_1[current_index] = ind_1;
      indexes_2[current_index] = ind_2;
      --ind_1;
      --ind_2;
      --current_index;
      continue;
    }
    if (matrix[ind_1][ind_2] == matrix[ind_1][ind_2 - 1]) {
      --ind_2;
      continue;
    }
    if (matrix[ind_1][ind_2] == matrix[ind_1 - 1][ind_2]) {
      --ind_1;
    }
  }
  for (auto& element : indexes_1) {
    std::cout << element << " ";
  }
  std::cout << "\n";
  for (auto& element : indexes_2) {
    std::cout << element << " ";
  }
  return 0;
}
