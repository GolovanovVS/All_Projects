#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
  int number;
  int all = 0;
  const int kTime = 42195;
  const int kSix = 6;
  std::string operation;
  std::unordered_map<int, int> user_time;
  std::cin >> number;
  std::vector<int> times(kTime);
  for (int i = 0; i < number; ++i) {
    std::cin >> operation;
    if (operation == "CHEER") {
      int user;
      std::cin >> user;
      if (!user_time.contains(user)) {
        std::cout << std::setprecision(kSix) << 0.0 << '\n';
        continue;
      }
      if (all == 1) {
        std::cout << std::setprecision(kSix) << 1.0 << '\n';
        continue;
      }
      int value = user_time[user];
      int count = 0;
      for (int j = 0; j < value; ++j) {
        count += times[j];
      }
      std::cout << std::setprecision(kSix) << (1.0 * count) / (all - 1) << '\n';
    } else if (operation == "RUN") {
      int user;
      int sec;
      std::cin >> user >> sec;
      if (user_time.contains(user)) {
        times[user_time[user]] -= 1;
        user_time[user] = sec;
      } else {
        user_time.insert_or_assign(user, sec);
        ++all;
      }
      times[sec] += 1;
    }
  }
  return 0;
}