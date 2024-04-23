#include <iostream>
#include <vector>

std::vector<int> order;

bool CreateTop(int vvv, std::vector<int>& color, std::vector<std::vector<int>>& son,
         std::vector<std::vector<int>>& parent) {
  color[vvv] = 1;
  for (auto& elem : parent[vvv]) {
    if (color[elem] == 1) {
      return false;
    }
    if (color[elem] == 0) {
      if (!CreateTop(elem, color, son, parent)) {
        return false;
      }
    }
  }
  color[vvv] = 2;
  order.push_back(vvv);
  return true;
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
    parent[vvv].push_back(uuu);
  }
  for (int i = 0; i < nnn; ++i) {
    if (color[i] == 0) {
      if (!CreateTop(i, color, son, parent)) {
        std::cout << -1;
        return 0;
      }
    }
  }
  for (auto& elem : order) {
    std::cout << elem + 1 << ' ';
  }
  return 0;
}
