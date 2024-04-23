#include <iostream>
#include <vector>
struct Fenwick {
  std::vector<std::vector<int>> matr;
  int size;
  void Init(int num) {
    size = num;
    matr.assign(num + 1, std::vector<int>(num + 1, 0));
  }
  void Upgrade(int fir, int sec) {
    for (int i = fir; i < size; i = i | (i + 1)) {
      for (int j = sec; j < size; j = j | (j + 1)) {
        matr[i][j]++;
      }
    }
  }
  int Get(int fir, int sec) {
    int result = 0;
    for (int i = fir; i > 0; i = (i & (i + 1)) - 1) {
      for (int j = sec; j > 0; j = (j & (j + 1)) - 1) {
        result += matr[i][j];
      }
    }
    return result;
  }
  int Get(int x1, int y1, int x2, int y2) {
    return Get(x2, y2) + Get(x1 - 1, y1 - 1) - Get(x1 - 1, y2) -
           Get(x2, y1 - 1);
  }
};
int main() {
  int number;
  int requests;
  int fir;
  int sec;
  std::string str;
  Fenwick tr;
  std::cin >> number >> requests;
  tr.Init(number + 2);
  for (int i = 0; i < requests; ++i) {
    std::cin >> str;
    if (str == "GET") {
      int x1;
      int y1;
      int x2;
      int y2;
      std::cin >> x1 >> y1 >> x2 >> y2;
      if (y1 > y2) {
        std::swap(y1, y2);
      }
      if (x1 > x2) {
        std::swap(x1, x2);
      }
      std::cout << tr.Get(x1, y1, x2, y2) << '\n';
      continue;
    }
    std::cin >> fir >> sec;
    tr.Upgrade(fir, sec);
  }
  return 0;
}