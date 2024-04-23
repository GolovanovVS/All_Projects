#include <cmath>
#include <iostream>
#include <vector>

bool Bit(int mask, int pos) { return ((mask >> pos) & 1) != 0; }

std::vector<std::vector<int>> FullDp(std::vector<std::vector<int>>& dp,
                                     const std::vector<std::vector<int>>& mat) {
  int am = dp[0].size();
  for (int mask = 0; mask < (int)std::pow(2, am); ++mask) {
    for (int j = 0; j < am; ++j) {
      if (mask == (int)std::pow(2, j)) {
        dp[mask][j] = 0;
      }
      for (int i = 0; i < am; ++i) {
        if (!Bit(mask, i)) {
          int new_mask = mask | (1 << i);
          dp[new_mask][i] = std::min(dp[new_mask][i], dp[mask][j] + mat[i][j]);
        }
      }
    }
  }
  return dp;
}

int main() {
  int am;
  const int kMax = 1e8 + 1;
  std::cin >> am;
  std::vector<std::vector<int>> mat(am, std::vector<int>(am));
  for (int i = 0; i < am; ++i) {
    for (int j = 0; j < am; ++j) {
      std::cin >> mat[i][j];
    }
  }
  std::vector<std::vector<int>> dp((int)std::pow(2, am),
                                   std::vector<int>(am, kMax));
  dp = FullDp(dp, mat);
  int min = kMax;
  int end = 0;
  for (int i = 0; i < am; ++i) {
    if (min > dp[(int)std::pow(2, am) - 1][i]) {
      min = dp[(int)std::pow(2, am) - 1][i];
      end = i;
    }
  }
  std::cout << min << '\n';
  std::vector<int> ans(1, end);
  int mask = (int)std::pow(2, am) - 1;
  for (int k = 0; k < am; ++k) {
    for (int i = 0; i < am; ++i) {
      if (Bit(mask, i) and
          (dp[mask][end] ==
           dp[mask - (int)std::pow(2, end)][i] + mat[i][end])) {
        mask -= (int)std::pow(2, end);
        end = i;
        ans.push_back(end);
      }
    }
  }
  for (auto& element : ans) {
    std::cout << element + 1 << ' ';
  }
  return 0;
}
