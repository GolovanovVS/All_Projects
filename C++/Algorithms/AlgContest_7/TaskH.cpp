#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

std::vector<std::vector<int>> graph;
const int KMax = 5'000;

void DFS(std::vector<int>& visited, std::vector<int>& tin,
         std::vector<int>& low, std::vector<int>& ans,
         std::vector<int>& values) {
  int vvv = values[0];
  int ppp = values[1];
  visited[vvv] = 1;
  tin[vvv] = low[vvv] = values[2]++;
  for (auto& rib : graph[vvv]) {
    if (rib == ppp) {
      continue;
    }
    if (visited[rib] == 1) {
      low[vvv] = std::min(low[vvv], tin[rib]);
    } else {
      std::vector<int> val{rib, vvv, values[2]};
      DFS(visited, tin, low, ans, val);
      low[vvv] = std::min(low[vvv], low[rib]);
      if (low[rib] > tin[vvv]) {
        if (rib < vvv) {
          ans.push_back(rib * KMax + vvv);
          continue;
        }
        ans.push_back(vvv * KMax + rib);
      }
    }
  }
}

void FindBridges(int n, std::vector<int>& ans) {
  std::vector<int> visited(n);
  std::vector<int> tin(n, -1);
  std::vector<int> low(n, -1);
  int timer = 0;
  for (int i = 0; i < n; ++i) {
    if (visited[i] == 0) {
      std::vector<int> values{i, -1, timer};
      DFS(visited, tin, low, ans, values);
    }
  }
}

bool Dfs1(std::vector<int>& visited, int vvv, int& amount,
          const std::unordered_map<int, int>& crats) {
  visited[vvv] = 1;
  for (auto& elem : graph[vvv]) {
    if (elem < vvv and crats.contains(KMax * elem + vvv)) {
      ++amount;
    } else if (elem > vvv and crats.contains(KMax * vvv + elem)) {
      ++amount;
    } else if (visited[elem] == 0) {
      Dfs1(visited, elem, amount, crats);
    }
  }
  visited[vvv] = 2;
  return amount == 1;
}

int main() {
  int nnn;
  int mmm;
  std::cin >> nnn >> mmm;
  graph.resize(nnn);
  std::unordered_map<int, int> crats;
  for (int i = 0; i < mmm; i++) {
    int uuu;
    int vvv;
    std::cin >> uuu >> vvv;
    if (uuu == vvv) {
      continue;
    }
    --uuu;
    --vvv;
    graph[uuu].push_back(vvv);
    graph[vvv].push_back(uuu);
  }
  std::vector<int> ans;
  FindBridges(nnn, ans);
  std::vector<int> visited(nnn);
  int fff = 0;
  for (auto& elem : ans) {
    crats[elem] = 1;
  }
  for (int i = 0; i < nnn; ++i) {
    if (visited[i] == 0) {
      int hhh = 0;
      fff += (int)Dfs1(visited, i, hhh, crats);
    }
  }
  std::cout << (fff + 1) / 2;
  return 0;
}
