#include <iostream>
#include <vector>

int main() {
  int amount;
  std::cin >> amount;
  const int kMod = 1e9 + 7;
  const int kSize = 1e6 + 1;
  std::vector<int> num(amount);
  for (int i = 0; i < amount; ++i) {
    std::cin >> num[i];
  }
  std::vector<long long> end(kSize);
  long long ans = 1;
  for (int val : num) {
    long long read = ans;
    ans = (2 * read - end[val] + kMod) % kMod;
    end[val] = read;
  }
  std::cout << (ans - 1) % kMod;
  return 0;
}
