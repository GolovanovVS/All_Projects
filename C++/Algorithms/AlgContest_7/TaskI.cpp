#include <cmath>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;
const int64_t KMax = 1'000'000;
int64_t fir = -1;

struct Cell {
  vector<int64_t> ribs{};
  int64_t koef_positive = KMax * KMax;
  int64_t koef_negative = KMax * KMax;
};

vector<Cell> graph;

void DFS(int64_t vvv, int64_t prev, vector<int64_t>& visited,
         unordered_map<int64_t, int64_t>& cost) {
  visited[vvv] = 1;
  if (prev == -1) {
    graph[vvv].koef_positive = 0;
  } else {
    if (graph[prev].koef_positive != KMax * KMax) {
      graph[vvv].koef_negative =
          cost[KMax * (int64_t)vvv + (int64_t)prev] - graph[prev].koef_positive;
    }
    if (graph[prev].koef_negative != KMax * KMax) {
      graph[vvv].koef_positive =
          cost[KMax * (int64_t)vvv + (int64_t)prev] - graph[prev].koef_negative;
    }
  }
  for (auto& elem : graph[vvv].ribs) {
    if (elem == prev) {
      continue;
    }
    if (visited[elem] == 0) {
      DFS(elem, vvv, visited, cost);
    } else {
      if (graph[vvv].koef_positive != KMax * KMax) {
        graph[elem].koef_negative = cost[KMax * (int64_t)elem + (int64_t)vvv] -
                                    graph[vvv].koef_positive;
      }
      if (graph[vvv].koef_negative != KMax * KMax) {
        graph[elem].koef_positive = cost[KMax * (int64_t)elem + (int64_t)vvv] -
                                    graph[vvv].koef_negative;
      }
      if (graph[elem].koef_positive != (KMax * KMax) and
          graph[elem].koef_negative != (KMax * KMax)) {
        fir = (graph[elem].koef_negative - graph[elem].koef_positive) / 2;
      }
    }
  }
}

bool IsPossible(int64_t fff) {
  vector<int64_t> are(graph.size());
  for (int64_t i = 1; i < (int64_t)are.size(); ++i) {
    int64_t res = 0;
    if (graph[i].koef_positive != KMax * KMax) {
      res = (int64_t)graph[i].koef_positive + fff;
    }
    if (graph[i].koef_negative != KMax * KMax) {
      res = (int64_t)graph[i].koef_negative - fff;
    }
    if (((int64_t)graph.size() <= res) or (res < 1)) {
      return false;
    }
    ++are[res];
    if (are[res] > 1) {
      return false;
    }
  }
  return true;
}

int main() {
  int64_t nnn;
  int64_t mmm;
  cin >> nnn >> mmm;
  unordered_map<int64_t, int64_t> cost;
  vector<int64_t> visited(nnn + 1);
  graph.resize(nnn + 1);
  for (int64_t i = 0; i < mmm; ++i) {
    int64_t uuu;
    int64_t vvv;
    int64_t ccc;
    cin >> uuu >> vvv >> ccc;
    cost[KMax * uuu + vvv] = ccc;
    cost[KMax * vvv + uuu] = ccc;
    graph[vvv].ribs.push_back((int64_t)uuu);
    graph[uuu].ribs.push_back((int64_t)vvv);
  }
  DFS(1, -1, visited, cost);
  vector<int64_t> res(nnn + 1);
  if (fir == -1) {
    int64_t min1 = 2 * KMax;
    int64_t min2 = 2 * KMax;
    for (int64_t i = 1; i < nnn + 1; ++i) {
      min2 = std::min(min2, (int64_t)graph[i].koef_positive);
      min1 = std::min(min1, (int64_t)graph[i].koef_negative);
    }
    if (IsPossible((int64_t)(1 - min2))) {
      fir = 1 - min2;
    }
    if (IsPossible((int64_t)min1 - 1)) {
      fir = min1 - 1;
    }
  }
  for (int64_t i = 1; i < nnn + 1; ++i) {
    if (graph[i].koef_positive != KMax * KMax) {
      res[i] = (int64_t)graph[i].koef_positive + fir;
    }
    if (graph[i].koef_negative != KMax * KMax) {
      res[i] = (int64_t)graph[i].koef_negative - fir;
    }
  }
  for (int64_t i = 1; i < nnn + 1; ++i) {
    cout << res[i] << ' ';
  }
  return 0;
}
