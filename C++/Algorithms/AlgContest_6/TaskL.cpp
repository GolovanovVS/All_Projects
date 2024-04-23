#include <bits/stdc++.h>

using namespace std;

int main() {
  int num;
  int gro;
  int cost;
  const int kMax = 100000;
  cin >> num >> gro >> cost;
  vector<vector<vector<int>>> val(gro + 1);
  for (int i = 0; i < num; ++i) {
    vector<int> read(2);
    int in;
    cin >> read[0] >> read[1] >> in;
    val[in].push_back(read);
  }
  vector<vector<int>> dp(gro + 1, vector<int>(cost + 1));
  for (int i = 0; i < gro + 1; ++i) {
    for (int j = 0; j < cost + 1; ++j) {
      if (j == 0) {
        dp[i][j] = 0;
        continue;
      }
      if (i == 0) {
        dp[i][j] = -kMax;
        continue;
      }
      dp[i][j] = dp[i - 1][j];
      for (auto& vec : val[i]) {
        if (vec[0] <= j) {
          dp[i][j] = max(dp[i][j], dp[i - 1][j - vec[0]] + vec[1]);
        }
      }
    }
  }
  int mm = 0;
  for (int i = 0; i < cost + 1; ++i) {
    mm = max(mm, dp[gro][i]);
  }
  cout << mm;
  return 0;
}
