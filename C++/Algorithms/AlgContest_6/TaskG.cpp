#include <iostream>
#include <vector>

int main() {
  int am;
  std::cin >> am;
  std::vector<int> num(am + 1);
  for (int i = 1; i < am + 1; ++i) {
    std::cin >> num[i];
  }
  std::vector<int> up(am + 1);
  std::vector<int> inu(am + 1);
  std::vector<int> down(am + 1);
  std::vector<int> indo(am + 1);
  for (int i = 1; i < am + 1; ++i) {
    for (int j = 1; j < i; ++j) {
      if ((num[j] < num[i]) and (up[i] < down[j])) {
        up[i] = down[j];
        inu[i] = j;
      } else if ((num[j] > num[i]) and (down[i] < up[j])) {
        down[i] = up[j];
        indo[i] = j;
      }
    }
    ++up[i];
    ++down[i];
  }
  int max = 0;
  int ind = 0;
  bool type = false;
  for (int k = 0; k < am + 1; ++k) {
    if (max < up[k]) {
      max = up[k];
      ind = k;
      type = false;
    }
    if (max < down[k]) {
      max = down[k];
      ind = k;
      type = true;
    }
  }
  std::vector<int> ans;
  for (int i = 0; i < max; ++i) {
    if (type) {
      ans.push_back(num[ind]);
      ind = indo[ind];
      type = false;
      continue;
    }
    if (!type) {
      ans.push_back(num[ind]);
      ind = inu[ind];
      type = true;
    }
  }
  std::cout << max << '\n';
  for (int i = ans.size() - 1; i > -1; --i) {
    std::cout << ans[i] << ' ';
  }
  return 0;
}
