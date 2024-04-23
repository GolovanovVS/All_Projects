#include <algorithm>
#include <iostream>
#include <vector>

const int kMax = 100000;

std::vector<int> Backpack(const std::vector<std::vector<int>>& value,
                          int amount, int weight) {
  std::vector<std::vector<int>> dp(amount + 1, std::vector<int>(weight + 1));
  for (int i = 0; i < amount + 1; ++i) {
    for (int j = 0; j < weight + 1; ++j) {
      if (j == 0) {
        dp[i][j] = 0;
        continue;
      }
      if (i == 0) {
        dp[i][j] = -kMax;
        continue;
      }
      if (j < value[i - 1][0]) {
        dp[i][j] = dp[i - 1][j];
        continue;
      }
      dp[i][j] = std::max(dp[i - 1][j],
                          dp[i - 1][j - value[i - 1][0]] + value[i - 1][1]);
    }
  }
  int max = 0;
  int amount_index = amount;
  int weight_index = 0;
  for (int i = 0; i < weight + 1; ++i) {
    if (max < dp[amount][i]) {
      max = dp[amount][i];
      weight_index = i;
    }
  }
  std::vector<int> answer;
  while (dp[amount_index][weight_index] > 0) {
    if (dp[amount_index - 1][weight_index] == dp[amount_index][weight_index]) {
      --amount_index;
      continue;
    }
    weight_index -= value[amount_index - 1][0];
    answer.push_back(--amount_index);
  }
  std::reverse(answer.begin(), answer.end());
  return answer;
}

int main() {
  int amount;
  int weight;
  std::cin >> amount >> weight;
  std::vector<std::vector<int>> value(amount, std::vector<int>(2));
  for (int i = 0; i < amount; ++i) {
    std::cin >> value[i][0];
  }
  for (int i = 0; i < amount; ++i) {
    std::cin >> value[i][1];
  }
  std::vector<int> answer = Backpack(value, amount, weight);
  for (auto& element : answer) {
    std::cout << element + 1 << '\n';
  }
  return 0;
}
