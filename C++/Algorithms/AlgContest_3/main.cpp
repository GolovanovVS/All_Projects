#include <algorithm>
#include <iostream>
#include <vector>

const int kMaxn = 1010;

int tr[kMaxn][kMaxn];
int size = 0;

void Update(int lx, int rx) {
  for (int i = lx; i <= size; i |= (i + 1)) {
    for (int j = rx; j <= size; j |= (j + 1)) {
      tr[i][j]++;
    }
  }
}

int Get(int lx, int rx) {
  int ans = 0;
  for (int i = lx; i > 0; i = (i & (i + 1)) - 1) {
    for (int j = rx; j > 0; j = (j & (j + 1)) - 1) {
      ans += tr[i][j];
    }
  }
  return ans;
}

int Get(int x1, int y1, int x2, int y2) {
  return Get(x2, y2) - Get(x2, y1) - Get(x1, y2) + Get(x1, y1);
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);
  int qu;
  std::cin >> size >> qu;
  for (int i = 0; i < qu; ++i) {
    std::string str;
    std::cin >> str;
    if (str == "ADD") {
      int lx;
      int rx;
      std::cin >> lx >> rx;
      Update(lx, rx);
    } else {
      int x1;
      int x2;
      int y1;
      int y2;
      std::cin >> x1 >> y1 >> x2 >> y2;
      if (y1 > y2) {
        std::swap(y1, y2);
      }
      if (x1 > x2) {
        std::swap(x1, x2);
      }
      --x1;
      --y1;
      std::cout << Get(x1, y1, x2, y2) << '\n';
    }
  }
}