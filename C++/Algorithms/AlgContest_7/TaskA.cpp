#include <iostream>
#include <vector>

std::vector<int> rre;

void CreateTop(int vvv, std::vector<int>& color, std::vector<std::vector<int>>& son,
         std::vector<std::vector<int>>& parent) {
  color[vvv] = 1;
  for (int elem : son[vvv]) {
    if (color[elem] == 1 && rre.empty()) {
      int cut = elem;
      rre.push_back(elem);
      while (cut != vvv) {
        for (int uuu : son[cut]) {
          if (color[uuu] == 1) {
            rre.push_back(uuu);
            cut = uuu;
            break;
          }
        }
      }
    }
    if (color[elem] == 0) {
      CreateTop(elem, color, son, parent);
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
    parent[vvv].push_back(uuu);
  }
  for (int i = 0; i < nnn; ++i) {
    if (color[i] == 0) {
      CreateTop(i, color, son, parent);
    }
  }
  if (rre.empty()) {
    std::cout << "NO";
  } else {
    std::cout << "YES" << '\n';
    for (auto& elem : rre) {
      std::cout << elem + 1 << ' ';
    }
  }
  return 0;
}
