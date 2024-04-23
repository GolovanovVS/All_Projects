#include <iostream>
#include <vector>

void DFS(int vvv, std::vector<int>& color, std::vector<std::vector<int>>& son,
         std::vector<std::vector<int>>& parent, std::vector<int>& cur) {
  color[vvv] = 1;
  cur.push_back(vvv);
  for (int elem : son[vvv]) {
    if (color[elem] == 0) {
      DFS(elem, color, son, parent, cur);
    }
  }
  color[vvv] = 2;
}

int main() {
  int nnn;
  int mmm;
  std::cin >> nnn >> mmm;
  std::vector<std::vector<int>> son(nnn);
  std::vector<std::vector<int>> parent(nnn);
  std::vector<int> color(nnn);
  for (int i = 0; i < mmm; ++i) {
    int uuu;
    int vvv;
    std::cin >> uuu >> vvv;
    uuu--;
    vvv--;
    son[uuu].push_back(vvv);
    son[vvv].push_back(uuu);
    parent[vvv].push_back(uuu);
    parent[uuu].push_back(vvv);
  }
  std::vector<std::vector<int>> res;
  for (int i = 0; i < nnn; ++i) {
    if (color[i] == 0) {
      std::vector<int> cur;
      DFS(i, color, son, parent, cur);
      res.push_back(cur);
    }
  }
  std::cout << res.size() << '\n';
  for (auto& elem : res) {
    std::cout << elem.size() << '\n';
    for (auto& fff : elem) {
      std::cout << fff + 1 << ' ';
    }
    std::cout << '\n';
  }
  return 0;
}
