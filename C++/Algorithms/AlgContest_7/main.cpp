#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

class Graph {
 private:
  const int KMax = 200'000;
  struct Node {
    bool visited;
    int tin;
    int low;
    std::vector<int> edges;
  };

  std::vector<Node> graph_;
  std::unordered_map<int, std::vector<int>> edges_;

  void DFS(std::vector<int>& prev_result, int node, int son, int timer) {
    graph_[node].visited = true;
    graph_[node].tin = graph_[node].low = timer++;
    for (const auto& vertex : graph_[node].edges) {
      if (vertex == son) {
        continue;
      }
      if (graph_[vertex].visited) {
        graph_[node].low = std::min(graph_[node].low, graph_[vertex].tin);
      } else {
        DFS(prev_result, vertex, node, timer);
        graph_[node].low = std::min(graph_[node].low, graph_[vertex].low);
        if (graph_[vertex].low > graph_[node].tin) {
          if (vertex < node) {
            prev_result.push_back(vertex * KMax + node);
            continue;
          }
          prev_result.push_back(node * KMax + vertex);
        }
      }
    }
  }

 public:
  explicit Graph(int nodes) : graph_(std::vector<Node>(nodes)) {}
  void AddEdge(int node1, int node2, int number) {
    if (node1 == node2) {
      return;
    }
    if (--node1 < --node2) {
      edges_[KMax * node1 + node2].push_back(number);
    } else {
      edges_[KMax * node2 + node1].push_back(number);
    }
    graph_[node1].edges.push_back(node2);
    graph_[node2].edges.push_back(node1);
  }
  std::vector<int> FindBridges() {
    int nodes = static_cast<int>(graph_.size());
    std::vector<int> prev_result;
    std::vector<Node> vertexes(nodes, {false, -1, -1, {}});
    std::vector<int> visited(nodes);
    int timer = 0;
    for (int i = 0; i < nodes; ++i) {
      if (visited[i] == 0) {
        DFS(prev_result, i, -1, timer);
      }
    }
    std::vector<int> result;
    for (auto& element : prev_result) {
      if (edges_[element].size() == 1) {
        result.push_back(edges_[element][0]);
      }
    }
    sort(result.begin(), result.end());
    return result;
  }
};

int main() {
  int nodes;
  int amount_edges;
  std::cin >> nodes >> amount_edges;
  Graph graph = Graph(nodes);
  for (int i = 0; i < amount_edges; i++) {
    int node1;
    int node2;
    std::cin >> node1 >> node2;
    graph.AddEdge(node1, node2, i);
  }
  std::vector<int> result = graph.FindBridges();
  std::cout << result.size() << '\n';
  for (auto& element : result) {
    std::cout << element + 1 << ' ';
  }
  return 0;
}
