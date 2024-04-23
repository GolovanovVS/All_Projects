#include <cmath>
#include <iostream>
#include <vector>

bool Bit(int mask, int pos) { return ((mask >> pos) & 1) != 0; }

std::vector<int> FullDp(std::vector<int>& dp,
                        const std::vector<std::vector<int>>& mat) {
  int am = mat.size();
  for (int mask = 0; mask < (int)std::pow(2, am); ++mask) {
    for (int i = 0; i < am; ++i) {
      for (int j = 0; j < am; ++j) {
        if (mat[i][j] == 0) {
          continue;
        }
        if (!Bit(mask, i) and !Bit(mask, j)) {
          int mask1 = mask | (1 << j);
          int new_mask = mask1 | (1 << i);
          dp[new_mask] = std::max(dp[new_mask], dp[mask] + 2);
        }
      }
    }
  }
  return dp;
}

int main() {
  int am;
  std::cin >> am;
  std::vector<std::vector<int>> mat(am, std::vector<int>(am));
  for (int i = 0; i < am; ++i) {
    for (int j = 0; j < am; ++j) {
      char sym;
      std::cin >> sym;
      if (sym == 'N') {
        mat[i][j] = 0;
      } else {
        mat[i][j] = 1;
      }
    }
  }
  std::vector<int> dp(1 << am);
  dp = FullDp(dp, mat);
  std::cout << dp[(1 << am) - 1];
  return 0;
}
