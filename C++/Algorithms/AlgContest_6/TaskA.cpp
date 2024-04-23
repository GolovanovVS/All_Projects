#include <bits/stdc++.h>

int changeDp(int num, std::vector<int>& dp) {
  int l = 0;
  int r = (int)dp.size() - 1;
  while ((r - l) > 1) {
    int mid = (l + r) / 2;
    if (dp[mid] > num) {
      r = mid;
    }
    else {
      l = mid;
    }
  }
  dp[l] = num;
  return l;
}


int main() {
  int n;
  const int kMax = (int)std::pow(2, 31) - 1;
  std::cin >> n;
  std::vector<int> num(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> num[i];
  }
  std::vector<int> dp(n + 1, kMax);
  for (int i = 0; i < n; ++i) {
    changeDp(num[i], dp);
  }
  std::cout << changeDp(kMax, dp);
  return 0;
}
