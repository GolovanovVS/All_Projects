#include <bits/stdc++.h>

class Matrix {
  private:
  std::vector<std::vector<int>> matrix_;
  int mm_;
  int nn_;

  public:
  Matrix(std::vector<std::vector<int>> other) : mm_(other.size()), nn_(other[0].size()), matrix_(std::vector<std::vector<int>>(other.size(), std::vector<int>(other[0].size()))) {
    for (int i = 0; i < mm_; ++i) {
      for (int j = 0; j < nn_; ++j) {
        matrix_[i][j] = other[i][j];
      }
    }
  }

  Matrix(int a, int b) : mm_(a), nn_(b), matrix_(std::vector<std::vector<int>>(a, std::vector<int>(b))) {
  }

  Matrix operator=(const Matrix& other) {
    if (this == &other) {
      return *this;
    }
    mm_ = other.mm_;
    nn_ = other.nn_;
    matrix_.resize(mm_);
    for (int i = 0; i < mm_; ++i) {
      matrix_[i].resize(nn_);
      for (int j = 0; j < nn_; ++j) {
        matrix_[i][j] = other.matrix_[i][j];
      }
    }
    return *this;
  }

  Matrix operator*=(const Matrix& other) {
    if (nn_ != other.mm_) {
      std::cout << ("Tobi pizda");
      return *this;
    }
    Matrix result(mm_, other.nn_);
    for (int i = 0; i < result.mm_; ++i) {
      for (int j = 0; j < result.nn_; ++j) {
        int sum = 0;
        for (int l = 0; l < nn_; ++l) {
          sum += (matrix_[i][l] * other.matrix_[l][j]);
        }
        result.matrix_[i][j] = sum;
      }
    }
    *this = result;
    return *this;
  }

  Matrix Pow(int num) {
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
};

Matrix operator*(const Matrix& mat1, const Matrix& mat2) {
  Matrix result = mat1;
  result *= mat2;
  return result;
}

std::ostream& operator<<(std::ostream& output, const Matrix& mat) {
  for (int i = 0; i < mat.mm_; ++i) {
    for (int j = 0; j < mat.nn_; ++j) {
      output << mat.matrix_[i][j] << ' ';
    }
    output << '\n';
  }
  return output;
}

int main() {
  std::vector<std::vector<int>> mat{{2, 1, 1}, {1, 1, 1}, {1, 1, 1}};
  Matrix m = Matrix(mat);
  std::cout << m.Pow(2);
  return 0;
}