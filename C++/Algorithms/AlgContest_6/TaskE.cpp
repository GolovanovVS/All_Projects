#include <bits/stdc++.h>

int main() {
  int number;
  std::cin >> number;
  std::vector<std::vector<std::vector<long long>>> dp(
          number + 1, std::vector<std::vector<long long>>(
                              number + 1, std::vector<long long>(2)));
  for (int i = 1; i < number + 1; ++i) {
    for (int j = 1; j < number + 1; ++j) {
      if (i < j) {
        dp[i][j][0] = 0;
      }
      if (i == j) {
        dp[i][j][0] = 1;
      }
      if (i > j) {
        dp[i][j][0] = dp[i - j][j / 2][1];
      }
      dp[i + 1][j][1] = dp[i][j - 1][1] + dp[i][j][0];
    }
  }
  std::cout << dp[number][number][1];
  return 0;
}
