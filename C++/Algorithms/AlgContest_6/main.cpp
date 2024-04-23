#include <iostream>
#include <vector>

const int kMax = 100000;

std::vector<int> Backpack(const std::vector<std::vector<int>>& val, int amount,
                          int weight) {
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
      if (j < val[i - 1][0]) {
        dp[i][j] = dp[i - 1][j];
        continue;
      }
      dp[i][j] =
          std::max(dp[i - 1][j], dp[i - 1][j - val[i - 1][0]] + val[i - 1][1]);
    }
  }
  int max = 0;
  int amount_ind = amount;
  int weight_ind = 0;
  for (int k = 0; k < weight + 1; ++k) {
    if (max < dp[amount][k]) {
      max = dp[amount][k];
      weight_ind = k;
    }
  }
  std::vector<int> answer;
  while (dp[amount_ind][weight_ind] > 0) {
    if (dp[amount_ind - 1][weight_ind] == dp[amount_ind][weight_ind]) {
      --amount_ind;
      continue;
    }
    weight_ind -= val[amount_ind - 1][0];
    answer.push_back(--amount_ind);
  }
  return answer;
}

int main() {
  int amount;
  int weight;
  std::cin >> amount >> weight;
  std::vector<std::vector<int>> val(amount, std::vector<int>(2));
  for (int i = 0; i < amount; ++i) {
    std::cin >> val[i][0];
  }
  for (int i = 0; i < amount; ++i) {
    std::cin >> val[i][1];
  }
  std::vector<int> answer = Backpack(val, amount, weight);
  for (int k = answer.size() - 1; k > -1; --k) {
    std::cout << answer[k] + 1 << '\n';
  }
  return 0;
}
