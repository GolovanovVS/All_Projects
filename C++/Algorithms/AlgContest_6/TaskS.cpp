#include <bits/stdc++.h>

int64_t mod;

class Matrix {
 private:
  int64_t mm_;
  int64_t nn_;
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

  Matrix operator=(Matrix& other) {
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
          sum +=
              (((matrix_[i][ind] % mod) * (other.matrix_[ind][j] % mod)) % mod);
          sum %= mod;
        }
        result.matrix_[i][j] = sum % mod;
      }
    }
    *this = result;
    return *this;
  }

  int64_t First() { return matrix_[0][0]; };

  Matrix Deg(const std::string& line) {
    const int64_t kDeg = 10;
    Matrix res = (*this).Pow((int64_t)(line[0] - '0'));
    Matrix cur = (*this).Pow(kDeg);
    for (int64_t i = 1; i < (int64_t)line.size(); ++i) {
      res *= cur.Pow((int64_t)(line[i] - '0'));
      cur *= cur.Pow(kDeg - 1);
    }
    return res;
  }

  Matrix Pow(int64_t num) {
    if (num == 0) {
      Matrix res = Matrix(mm_, mm_);
      for (int64_t i = 0; i < (int64_t)mm_; ++i) {
        res.matrix_[i][i] = 1;
      }
      return res;
    }
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

  int64_t Sum() {
    int64_t sum = 0;
    for (int64_t i = 0; i < (int64_t)mm_; ++i) {
      sum += matrix_[i][0];
      sum %= mod;
    }
    return sum;
  }

  friend std::ostream& operator<<(std::ostream& output, const Matrix& mat);
  friend Matrix Create(int64_t am);
};

Matrix operator*(const Matrix& mat1, const Matrix& mat2) {
  Matrix result = mat1;
  result *= mat2;
  return result;
}

std::ostream& operator<<(std::ostream& output, const Matrix& mat) {
  for (int64_t i = 0; i < (int64_t)mat.mm_; ++i) {
    for (int64_t j = 0; j < (int64_t)mat.nn_; ++j) {
      output << mat.matrix_[i][j] << ' ';
    }
    output << '\n';
  }
  return output;
}

bool Bit(int64_t mask, int64_t pos) { return ((mask >> pos) & 1) != 0; }

bool CanBeTogether(int64_t ind1, int64_t ind2, int64_t am) {
  for (int64_t i = 0; i < am - 1; ++i) {
    if ((Bit(ind1, i) == Bit(ind2, i)) and
        (Bit(ind1, i + 1) == Bit(ind2, i)) and
        (Bit(ind1, i) == Bit(ind2, i + 1))) {
      return false;
    }
  }
  return true;
}

Matrix Create(int64_t am) {
  int64_t nn = 1 << am;
  Matrix result = Matrix(nn, nn);
  for (int64_t i = 0; i < nn; ++i) {
    for (int64_t j = 0; j < nn; ++j) {
      if (CanBeTogether(i, j, am)) {
        result.matrix_[i][j] = 1;
      } else {
        result.matrix_[i][j] = 0;
      }
    }
  }
  return result;
}

void Change(std::string& other) {
  int64_t have = -1;
  for (int64_t i = (int64_t)other.size() - 1; i > -1 && have != 0; --i) {
    if (other[i] - '0' == 0) {
      other[i] = '9';
    } else {
      have = 0;
      other[i] = static_cast<int64_t>(other[i] - '0') - 1 + '0';
    }
  }
  if (other[0] == '0') {
    other = other.substr(1);
  }
}

int main() {
  std::string line;
  int64_t num;
  std::cin >> line >> num >> mod;
  Matrix res = Matrix(std::vector<int64_t>((1 << num), 1));
  Change(line);
  std::reverse(line.begin(), line.end());
  Matrix per = Create(num).Deg(line);
  per *= res;
  std::cout << per.Sum() % mod;
  return 0;
}
