#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

constexpr size_t cMaxHeight = 1001000LL;

struct Edge {
  size_t to;
  size_t weight;
};

using namespace std;
using Edges = vector<Edge>;
using Graph = vector<Edges>;
const size_t cKMax = 18446744073709551615UL;

vector<size_t> Dijkstra(const Graph& graph, size_t start) {
  vector<size_t> distances(graph.size(), cKMax);
  distances[start] = 0;

  using QueueElement = pair<size_t, size_t>;
  priority_queue<QueueElement, vector<QueueElement>, greater<>> priority_queue;

  priority_queue.push({0, start});

  while (!priority_queue.empty()) {
    auto [current_distance, currentNode] = priority_queue.top();
    priority_queue.pop();

    if (current_distance > distances[currentNode]) {
      continue;
    }

    for (const auto& [next_node, weight] : graph[currentNode]) {
      size_t new_distance = current_distance + weight;
      if (new_distance < distances[next_node]) {
        distances[next_node] = new_distance;
        priority_queue.push({new_distance, next_node});
      }
    }
  }

  return distances;
}

int main() {
  size_t n;
  size_t up_cost;
  size_t down_cost;
  size_t enter_cost;
  size_t exit_cost;
  size_t num_teleports;
  cin >> n >> up_cost >> down_cost >> enter_cost >> exit_cost >> num_teleports;

  Graph graph(cMaxHeight + num_teleports);
  for (size_t i = 0; i < cMaxHeight; ++i) {
    if (i > 0) {
      graph[i].push_back({i - 1, down_cost});
    }
    if (i + 1 < cMaxHeight) {
      graph[i].push_back({i + 1, up_cost});
    }
  }

  for (size_t i = 0; i < num_teleports; ++i) {
    size_t num_nodes;
    cin >> num_nodes;
    for (size_t j = 0; j < num_nodes; ++j) {
      size_t node;
      cin >> node;
      --node;
      graph[cMaxHeight + i].push_back({node, exit_cost});
      graph[node].push_back({cMaxHeight + i, enter_cost});
    }
  }

  auto distances = Dijkstra(graph, 0);
  cout << distances[n - 1] << '\n';
}
