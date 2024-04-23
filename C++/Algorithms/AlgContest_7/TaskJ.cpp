#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool Dfs(int64_t vvv, vector<bool>& visited,
         const vector<vector<pair<int64_t, int64_t>>>& graph,
         vector<bool>& color_edge, const string& mask) {
  visited[vvv] = true;
  bool color = (mask[vvv] - '1' == 0);
  for (auto ele : graph[vvv]) {
    int64_t elem = ele.first;
    if (!visited[elem]) {
        if (Dfs(elem, visited, graph, color_edge, mask)) {
        color_edge[ele.second] = true;
        color ^= 1;
      }
    }
  }
  return color;
}

void Input() {
  int64_t nnn;
  int64_t mmm;
  cin >> nnn >> mmm;
  vector<vector<pair<int64_t, int64_t>>> graph(nnn);
  vector<bool> visited(nnn);
  vector<bool> color_edge(mmm);
  string mask;
  for (long long i = 0; i < mmm; ++i) {
    int64_t uuu;
    int64_t vvv;
    cin >> uuu >> vvv;
    --uuu;
    --vvv;
    graph[uuu].push_back({vvv, i});
    graph[vvv].push_back({uuu, i});
  }
  cin >> mask;
  for (int64_t i = 0; i < nnn; ++i) {
    if (!visited[i]) {
      if (Dfs(i, visited, graph, color_edge, mask)) {
        cout << -1 << '\n';
        return;
      }
    }
  }
  for (int i = 0; i < (int)color_edge.size(); ++i) {
    cout << color_edge[i];
  }
  cout << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int64_t ttt;
  cin >> ttt;
  for (int64_t i = 0; i < ttt; ++i) {
    Input();
  }
  return 0;
}