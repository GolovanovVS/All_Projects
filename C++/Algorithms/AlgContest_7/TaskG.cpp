#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

std::vector<std::vector<int>> graph;
const int KMax = 200'000;

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

int main() {
  int nnn;
  int mmm;
  std::cin >> nnn >> mmm;
  graph.resize(nnn);
  std::unordered_map<int, std::vector<int>> crats;
  for (int i = 0; i < mmm; i++) {
    int uuu;
    int vvv;
    std::cin >> uuu >> vvv;
    if (uuu == vvv) {
      continue;
    }
    --uuu;
    --vvv;
    if (uuu < vvv) {
      crats[KMax * uuu + vvv].push_back(i);
    } else {
      crats[KMax * vvv + uuu].push_back(i);
    }
    graph[uuu].push_back(vvv);
    graph[vvv].emplace_back(uuu);
  }
  std::vector<int> ans;
  std::vector<int> res;
  FindBridges(nnn, ans);
  for (auto& elem : ans) {
    if (crats[elem].size() == 1) {
      res.push_back(crats[elem][0]);
    }
  }
  std::cout << res.size() << '\n';
  sort(res.begin(), res.end());
  for (auto& elem : res) {
    std::cout << elem + 1 << ' ';
  }
  return 0;
}
