#include <bits/stdc++.h>

int main() {
  int num_1;
  int num_2;
  int ans = 0;
  int count;
  std::cin >> num_1 >> num_2;
  std::vector<int> dp(num_2);
  std::vector<int> vec_1(num_1);
  std::vector<int> vec_2(num_2);
  for (auto& element : vec_1) {
    std::cin >> element;
  }
  for (auto& element : vec_2) {
    std::cin >> element;
  }
  for (int i = 0; i < num_1; ++i) {
    count = 0;
    for (int j = 0; j < num_2; ++j) {
      if (vec_2[j] < vec_1[i]) {
        count = std::max(count, dp[j]);
        continue;
      }
      if (vec_2[j] == vec_1[i]) {
        dp[j] = std::max(dp[j], count + 1);
        continue;
      }
    }
  }
  for (auto& element : dp) {
    ans = std::max(ans, element);
  }
  std::cout << ans;
}