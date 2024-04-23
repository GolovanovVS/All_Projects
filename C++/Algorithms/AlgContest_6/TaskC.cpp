#include <cmath>
#include <iostream>
#include <vector>

int main() {
  int height;
  int amount;
  std::cin >> height >> amount;
  int max_amount = std::ceil(log2(height));
  if (max_amount <= amount) {
    std::cout << max_amount;
    return 0;
  }
  if (amount == 0) {
    std::cout << -1;
    return 0;
  }
  std::vector<std::vector<int>> dp(height + 1, std::vector<int>(amount + 1));
  for (int i = 0; i < height + 1; ++i) {
    for (int j = 0; j < amount + 1; ++j) {
      if ((i * j) == 0) {
        dp[i][j] = 1;
      } else {
        dp[i][j] = dp[i - 1][j] + dp[i - 1][j - 1];
      }
    }
  }
  for (int i = 0; i < height + 1; ++i) {
    if (dp[i][amount] >= height) {
      std::cout << i;
      return 0;
    }
  }
  return 0;
}
