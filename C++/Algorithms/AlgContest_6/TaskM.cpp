#include <bits/stdc++.h>
using namespace std;

const int64_t kMod = 1e9 + 9;

class Matrix {
 private:
  size_t mm_;
  size_t nn_;
  std::vector<std::vector<int64_t>> matrix_;

 public:
  Matrix(std::vector<std::vector<int64_t>> other)
      : mm_(other.size()),
        nn_(other[0].size()),
        matrix_(std::vector<std::vector<int64_t>>(
            other.size(), std::vector<int64_t>(other[0].size()))) {
    for (int64_t i = 0; i < (int64_t)mm_; ++i) {
      for (int64_t j = 0; j < (int64_t)nn_; ++j) {
        matrix_[i][j] = other[i][j];
      }
    }
  }
  Matrix(const Matrix& other)
      : mm_(other.mm_),
        nn_(other.nn_),
        matrix_(std::vector<std::vector<int64_t>>(
            other.mm_, std::vector<int64_t>(other.nn_))) {
    for (int64_t i = 0; i < (int64_t)mm_; ++i) {
      for (int64_t j = 0; j < (int64_t)nn_; ++j) {
        matrix_[i][j] = other.matrix_[i][j];
      }
    }
  }
  Matrix(std::vector<int64_t> vec)
      : mm_(vec.size()),
        nn_(1),
        matrix_(std::vector<std::vector<int64_t>>(vec.size(),
                                                  std::vector<int64_t>(1))) {
    for (int64_t i = 0; i < (int64_t)mm_; ++i) {
      matrix_[i][0] = vec[i];
    }
  }
  Matrix(int64_t mm, int64_t nn)
      : mm_(mm),
        nn_(nn),
        matrix_(
            std::vector<std::vector<int64_t>>(mm, std::vector<int64_t>(nn))) {}

  Matrix& operator=(Matrix& other) {
    if (this == &other) {
      return *this;
    }
    mm_ = other.mm_;
    nn_ = other.nn_;
    matrix_.resize(mm_);
    for (int64_t i = 0; i < (int64_t)mm_; ++i) {
      matrix_[i].resize(nn_);
      for (int64_t j = 0; j < (int64_t)nn_; ++j) {
        matrix_[i][j] = other.matrix_[i][j];
      }
    }
    return *this;
  }

  Matrix operator*=(const Matrix& other) {
    if (nn_ != other.mm_) {
      std::cout << ("ERROR");
      return *this;
    }
    Matrix result(mm_, other.nn_);
    for (int64_t i = 0; i < (int64_t)result.mm_; ++i) {
      for (int64_t j = 0; j < (int64_t)result.nn_; ++j) {
        int64_t sum = 0;
        for (int64_t ind = 0; ind < (int64_t)nn_; ++ind) {
          sum += (((matrix_[i][ind] % kMod) * (other.matrix_[ind][j] % kMod)) %
                  kMod);
          sum %= kMod;
        }
        result.matrix_[i][j] = sum % kMod;
      }
    }
    *this = result;
    return *this;
  }

  Matrix Pow(int64_t num) {
    if (num == 1) {
      return *this;
    }
    if (num == 2) {
      Matrix result = *this;
      return result *= (*this);
    }
    if (num % 2 == 1) {
      Matrix result = (*this).Pow(num - 1);
      return result *= (*this);
    }
    return ((*this).Pow(num / 2)).Pow(2);
  };

  friend std::ostream& operator<<(std::ostream& output, const Matrix& mat);
  friend int64_t Fib(int64_t ind);
};

int64_t Fib(int64_t ind) {
  Matrix ex = Matrix({{1, 1}, {1, 0}});
  Matrix res = ex.Pow(ind);
  return (res.matrix_[1][0] + res.matrix_[1][1]) % kMod;
}

int64_t Answer(const vector<vector<int64_t>>& cor, int64_t cc) {
  int64_t ans = 0;
  for (const auto& vec : cor) {
    int64_t kk = vec[1] - vec[0] - cc;
    if (kk < 0) {
      continue;
    }
    if (kk == 0) {
      ans += (vec[2] % kMod);
      ans %= kMod;
      continue;
    }
    ans += (((Fib(kk) % kMod) * (vec[2] % kMod)) % kMod);
    ans %= kMod;
  }
  return ans % kMod;
}

int main() {
  int64_t num;
  int64_t que;
  cin >> num >> que;
  vector<vector<int64_t>> cor(num, vector<int64_t>(3));
  for (int64_t i = 0; i < num; ++i) {
    cin >> cor[i][0] >> cor[i][1] >> cor[i][2];
  }
  int64_t cc;
  for (int64_t i = 0; i < que; ++i) {
    cin >> cc;
    cout << Answer(cor, cc) << '\n';
  }
  return 0;
}
