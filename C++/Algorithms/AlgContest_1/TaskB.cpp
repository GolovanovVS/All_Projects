#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

int main() {
  const int kDigit = 9;
  int number;
  std::cin >> number;
  std::vector<double> degrees(number + 1);
  degrees[0] = 0;
  for (int i = 1; i <= number; ++i) {
    double read;
    std::cin >> read;
    degrees[i] = degrees[i - 1] + std::log(read);
  }
  int request;
  std::cin >> request;
  for (int i = 0; i < request; ++i) {
    int begin;
    int end;
    std::cin >> begin >> end;
    std::cout << std::setprecision(kDigit)
              << std::exp((degrees[end + 1] - degrees[begin]) /
                          (end + 1 - begin))
              << '\n';
  }
  return 0;
}
