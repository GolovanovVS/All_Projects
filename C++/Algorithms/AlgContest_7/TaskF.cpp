#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

vector<vector<int>> adj;
vector<bool> visited;
vector<int> tin;
vector<int> low;
set<int> result;
int timer;

void Dfs(int vvv, int ppp) {
  visited[vvv] = true;
  tin[vvv] = low[vvv] = timer++;
  int children = 0;
  for (int elem : adj[vvv]) {
    if (visited[elem]) {
      low[vvv] = min(low[vvv], tin[elem]);
    } else {
      Dfs(elem, vvv);
      low[vvv] = min(low[vvv], low[elem]);
      if ((low[elem] >= tin[vvv]) && (ppp != -1)) {
        result.insert(vvv);
      }
      ++children;
    }
  }
  if ((ppp == -1) && (children > 1)) {
    result.insert(vvv);
  }
}

int main() {
  int nnn;
  int mmm;
  cin >> nnn >> mmm;
  adj.assign(nnn, vector<int>());
  visited.assign(nnn, false);
  tin.assign(nnn, -1);
  low.assign(nnn, -1);
  timer = 0;
  for (int i = 0; i < mmm; i++) {
    int vvv;
    int uuu;
    cin >> vvv >> uuu;
    if (vvv == uuu) {
      continue;
    }
    --vvv;
    --uuu;
    adj[vvv].push_back(uuu);
    adj[uuu].push_back(vvv);
  }
  for (int i = 0; i < nnn; ++i) {
    if (!visited[i]) {
      Dfs(i, -1);
    }
  }
  cout << result.size() << "\n";
  vector<int> res;
  for (int elem : result) {
    res.push_back(elem + 1);
  }
  sort(res.begin(), res.end());
  for (auto& elem : res) {
    cout << elem << '\n';
  }
  return 0;
}
