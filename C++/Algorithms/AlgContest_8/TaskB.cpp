#include <iostream>
#include <limits>
#include <queue>
#include <vector>

constexpr size_t cMax = std::numeric_limits<size_t>::max();

using Edge = std::pair<size_t, size_t>;
using Graph = std::vector<std::vector<Edge>>;
using MinHeap = std::priority_queue<Edge, std::vector<Edge>, std::greater<>>;

int64_t Dijkstra(const Graph& graph, std::vector<size_t>& infection_dist,
                 size_t start, size_t target) {
  size_t n = graph.size();
  std::vector<size_t> dist(n, cMax);
  MinHeap pq;
  MinHeap infection_queue;

  for (size_t i = 0; i < n; ++i) {
    if (infection_dist[i] == 0) {
      infection_queue.push({0, i});
    }
  }

  dist[start] = 0;
  pq.push({0, start});

  while (!pq.empty()) {
    if (!infection_queue.empty() &&
        infection_queue.top().first <= pq.top().first) {
      auto [curr_dist, node] = infection_queue.top();
      infection_queue.pop();
      if (infection_dist[node] != curr_dist) {
        continue;
      }

      for (const auto& [next, weight] : graph[node]) {
        size_t new_dist = curr_dist + weight;
        if (infection_dist[next] > new_dist) {
          infection_dist[next] = new_dist;
          infection_queue.push({new_dist, next});
        }
      }
    } else {
      auto [curr_dist, node] = pq.top();
      pq.pop();
      if (dist[node] != curr_dist) {
        continue;
      }
      if (node == target) {
        return (infection_dist[target] <= curr_dist) ? cMax : curr_dist;
      }

      for (const auto& [next, weight] : graph[node]) {
        size_t new_dist = curr_dist + weight;
        if (dist[next] > new_dist) {
          dist[next] = new_dist;
          pq.push({new_dist, next});
        }
      }
    }
  }

  return cMax;
}

int main() {
  size_t nodes;
  size_t edges;
  size_t viruses;
  std::cin >> nodes >> edges >> viruses;
  std::vector<size_t> virus_dist(nodes, cMax);

  for (size_t i = 0; i < viruses; ++i) {
    size_t virus_node;
    std::cin >> virus_node;
    virus_dist[--virus_node] = 0;
  }

  Graph graph(nodes);
  for (size_t i = 0; i < edges; ++i) {
    size_t u;
    size_t v;
    size_t weight;
    std::cin >> u >> v >> weight;
    --u;
    --v;
    graph[u].emplace_back(v, weight);
    graph[v].emplace_back(u, weight);
  }

  size_t start;
  size_t target;
  std::cin >> start >> target;
  --start;
  --target;
  int64_t result = Dijkstra(graph, virus_dist, start, target);
  std::cout << result << '\n';
}
