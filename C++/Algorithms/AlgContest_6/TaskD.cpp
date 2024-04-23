#include <bits/stdc++.h>

int main() {
  int n;
  int m;
  std::vector<std::vector<std::string>> boys(1001);
  std::vector<std::vector<std::string>>  girls(1001);
  std::cin >> n;
  for (int i = 0; i < n; ++i) {
    std::string name;
    int len;
    std::cin >> name >> len;
    boys[len].push_back(name);
  }
  std::cin >> m;
  for (int i = 0; i < m; ++i) {
    std::string name;
    int len;
    std::cin >> name >> len;
    girls[len].push_back(name);
  }
  int len = 0;
  int bp = 1000;
  int gp = 1000;
  std::vector<std::vector<std::string>> answer(n);
  int i = 0;
  while (bp != 0) {
    while(boys[bp].empty()) {
      --bp;
    }
    int max = bp;
    answer[i].push_back(boys[bp][boys[bp].size() - 1]);
    boys[bp].pop_back();
    while (gp != 0 and answer[i].size() < 4) {
      while(girls[gp].empty()) {
        --gp;
      }
      max = std::max(max, gp);
      answer[i].push_back(girls[gp][girls[gp].size() - 1]);
      girls[gp].pop_back();
    }
    len += max;
    ++i;
  }
  std::cout << len << '\n';
  std::cout << n << '\n';
  for (int i = 0; i < n; ++i) {
    std::cout << "Taxi " << i + 1 << answer[i][0];
    if (answer[i].size() == 4) {
      std::cout << ", " << answer[i][1] << ", " << answer[i][2] << " and " << answer[i][3];
    }
    if (answer[i].size() == 3) {
      std::cout << ", " << answer[i][1] << " and " << answer[i][2];
    }
    if (answer[i].size() == 2) {
      std::cout << " and " << answer[i][1];
    }
    std::cout << ".";
  }
  return 0;
}
