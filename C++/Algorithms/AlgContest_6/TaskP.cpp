#include <cmath>
#include <iostream>
#include <vector>

const int kMod = 1e9 + 7;

int Bit(int mask, int pos) { return ((mask >> pos) & 1); }

bool Check(int index, int mask, const std::vector<std::vector<int>>& cor) {
  for (int i = 0; i < static_cast<int>(cor.size()); ++i) {
    if (cor[i][index] == 2) {
      continue;
    }
    if (cor[i][index] != Bit(mask, i)) {
      return false;
    }
  }
  return true;
}

bool Chess(int mask, int nn) {
  for (int i = 0; i < nn - 1; ++i) {
    if (Bit(mask, i) == Bit(mask, i + 1)) {
      return false;
    }
  }
  return true;
}

bool CanBeUsed(int mask, const std::vector<std::vector<int>>& cor) {
  int length = static_cast<int>(cor.size());
  int width = static_cast<int>(cor[0].size());
  int other_mask = (1 << length) - mask - 1;
  for (int i = 0; i < width; ++i) {
    if (i % 2 == 0) {
      if (!Check(i, mask, cor)) {
        return false;
      }
    } else {
      if (!Check(i, other_mask, cor)) {
        return false;
      }
    }
  }
  return true;
}

int main() {
  int length;
  int width;
  std::cin >> length >> width;
  std::vector<std::vector<int>> cor(length, std::vector<int>(width));
  for (int i = 0; i < length; ++i) {
    for (int j = 0; j < width; ++j) {
      char symbol;
      std::cin >> symbol;
      if (symbol == '.') {
        cor[i][j] = 2;
      } else if (symbol == '-') {
        cor[i][j] = 0;
      } else {
        cor[i][j] = 1;
      }
    }
  }
  long long result = 1;
  int answer = 0;
  int possible_mask = 0;
  length = std::abs(length);
  for (int mask = 0; mask < static_cast<int>(1 << length); ++mask) {
    if (!Chess(mask, length)) {
      answer += static_cast<int>(CanBeUsed(mask, cor));
      continue;
    }
    possible_mask = mask;
  }
  for (int i = 0; i < width; ++i) {
    result *=
        (static_cast<int>(Check(i, possible_mask, cor)) +
         static_cast<int>(Check(i, ((1 << length) - possible_mask - 1), cor)));
    result %= kMod;
  }
  answer += result;
  std::cout << answer % kMod;
  return 0;
}