#include <iostream>
#include <vector>

void TopSort(int node, std::vector<std::vector<int>>& graph,
             std::vector<int>& visited, std::vector<int>& order) {
  visited[node] = 1;
  for (int son : graph[node]) {
    if (visited[son] == 0) {
      TopSort(son, graph, visited, order);
    }
  }
  order.push_back(node);
  visited[node] = 2;
}

void Dfs(int node, int index,
         const std::vector<std::vector<int>>& reversed_graph,
         std::vector<int>& visited, std::vector<int>& component) {
  visited[node] = 1;
  component[node] = index;
  for (int uuu : reversed_graph[node]) {
    if (visited[uuu] == 0) {
      Dfs(uuu, index, reversed_graph, visited, component);
    }
  }
  visited[node] = 2;
}

int main() {
  int nodes;
  int edges;
  std::cin >> nodes >> edges;
  std::vector<std::vector<int>> graph(nodes);
  std::vector<std::vector<int>> reversed_graph(nodes);
  std::vector<int> visited(nodes);
  std::vector<int> component(nodes, -1);
  std::vector<int> order;
  for (int i = 0; i < edges; ++i) {
    int node1;
    int node2;
    std::cin >> node1 >> node2;
    --node1;
    --node2;
    graph[node1].push_back(node2);
    reversed_graph[node2].push_back(node1);
  }
  for (int i = 0; i < nodes; ++i) {
    if (visited[i] == 0) {
      TopSort(i, graph, visited, order);
    }
  }
  fill(visited.begin(), visited.end(), 0);
  int index = 1;
  while (!order.empty()) {
    int node = order[order.size() - 1];
    order.pop_back();
    if (visited[node] == 0) {
      Dfs(node, index++, reversed_graph, visited, component);
    }
  }
  std::cout << index - 1 << '\n';
  for (auto& element : component) {
    std::cout << element << " ";
  }
  return 0;
}