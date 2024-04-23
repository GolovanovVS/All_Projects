#include <bits/stdc++.h>

using namespace std;
const long long kMod = 1e9 + 7;

class Matrix {
 private:
  size_t mm_;
  size_t nn_;
  std::vector<std::vector<long long>> matrix_;

 public:
  Matrix(std::vector<std::vector<long long>> other)
      : mm_(other.size()),
        nn_(other[0].size()),
        matrix_(std::vector<std::vector<long long>>(
            other.size(), std::vector<long long>(other[0].size()))) {
    for (long long i = 0; i < (long long)mm_; ++i) {
      for (long long j = 0; j < (long long)nn_; ++j) {
        matrix_[i][j] = other[i][j];
      }
    }
  }
  Matrix(const Matrix& other)
      : mm_(other.mm_),
        nn_(other.nn_),
        matrix_(std::vector<std::vector<long long>>(
            other.mm_, std::vector<long long>(other.nn_))) {
    for (long long i = 0; i < (long long)mm_; ++i) {
      for (long long j = 0; j < (long long)nn_; ++j) {
        matrix_[i][j] = other.matrix_[i][j];
      }
    }
  }
  Matrix(std::vector<long long> vec)
      : mm_(vec.size()),
        nn_(1),
        matrix_(std::vector<std::vector<long long>>(
            vec.size(), std::vector<long long>(1))) {
    for (long long i = 0; i < (long long)mm_; ++i) {
      matrix_[i][0] = vec[i];
    }
  }
  Matrix(long long mm, long long nn)
      : mm_(mm),
        nn_(nn),
        matrix_(std::vector<std::vector<long long>>(
            mm, std::vector<long long>(nn))) {}

  Matrix& operator=(Matrix& other) {
    if (this == &other) {
      return *this;
    }
    mm_ = other.mm_;
    nn_ = other.nn_;
    matrix_.resize(mm_);
    for (long long i = 0; i < (long long)mm_; ++i) {
      matrix_[i].resize(nn_);
      for (long long j = 0; j < (long long)nn_; ++j) {
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
    for (long long i = 0; i < (long long)result.mm_; ++i) {
      for (long long j = 0; j < (long long)result.nn_; ++j) {
        long long sum = 0;
        for (long long ind = 0; ind < (long long)nn_; ++ind) {
          sum += ((matrix_[i][ind] * other.matrix_[ind][j]) % kMod);
        }
        result.matrix_[i][j] = sum % kMod;
      }
    }
    *this = result;
    return *this;
  }

  long long First() { return matrix_[0][0]; };

  Matrix Pow(long long num) {
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
  friend Matrix Create(long long n);
  friend void NewWay(Matrix& way, long long hei, long long n);
};

Matrix operator*(const Matrix& mat1, const Matrix& mat2) {
  Matrix result = mat1;
  result *= mat2;
  return result;
}

std::ostream& operator<<(std::ostream& output, const Matrix& mat) {
  for (long long i = 0; i < (long long)mat.mm_; ++i) {
    for (long long j = 0; j < (long long)mat.nn_; ++j) {
      output << mat.matrix_[i][j] << ' ';
    }
    output << '\n';
  }
  return output;
}

Matrix Create(long long n) {
  Matrix result = Matrix(n, n);
  if (n == 1) {
    result.matrix_[0][0] = 1;
    return result;
  }
  for (long long i = 0; i < n; ++i) {
    result.matrix_[i][i] = 1;
    if (i == 0) {
      result.matrix_[i][i + 1] = 1;
      continue;
    }
    if (i == n - 1) {
      result.matrix_[i][i - 1] = 1;
      continue;
    }
    result.matrix_[i][i + 1] = 1;
    result.matrix_[i][i - 1] = 1;
  }
  return result;
}

void NewWay(Matrix& way, long long hei, long long n) {
  way.matrix_.resize(hei, vector<long long>(1, 0));
  way.mm_ = hei;
  Matrix temp = Create(hei);
  Matrix result = temp.Pow(n - 1);
  result *= way;
  way = result;
}

int main() {
  long long am;
  long long fin;
  cin >> am >> fin;
  vector<vector<long long>> cor(am, vector<long long>(3));
  for (long long i = 0; i < am; ++i) {
    cin >> cor[i][0] >> cor[i][1] >> cor[i][2];
  }
  vector<long long> tt(cor[0][2] + 1);
  tt[0] = 1;
  Matrix way = Matrix(tt);
  for (long long i = 0; i < am - 1; ++i) {
    NewWay(way, cor[i][2] + 1, cor[i][1] - cor[i][0] + 1);
  }
  NewWay(way, cor[am - 1][2] + 1, fin - cor[am - 1][0] + 1);
  cout << way.First();
  return 0;
}
