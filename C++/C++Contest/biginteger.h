#include <algorithm>
#include <cmath>
#include <deque>
#include <iostream>

enum class Sign { positive = 1, neutral = 0, negative = -1 };

class BigInteger {
  private:
  const int amount_of_digits = 9;
  const int max_digit = 1e9;// порядок системы счисления
  std::deque<int64_t> digits{0};// хранятся разряды в max_digit ричной системе счисления
  bool is_positive = true;      // -1 при отрицательных числах, иначе 1



  int del(const BigInteger& num) {
    if (*this < num) {
      return 0;
    }
    int right;
    int left;
    int64_t fir;
    int64_t sec = num.digits.back();
    if (num.digits.size() == digits.size() and !num.digits.empty()) {
      fir = digits.back();
    } else {
      fir = 1ll * digits.back() * max_digit + digits[digits.size() - 2];
    }
    left = fir / (sec + 1) - 1;
    right = (fir + 1) / sec + 1;
    if (left < 0) {
      left = 0;
    }
    if (right >= max_digit) {
      right = max_digit;
    }
    while (left < right - 1) {
      int middle = (left + right) / 2;
      BigInteger copy = num;
      copy *= middle;
      if (copy <= *this) {
        left = middle;
      } else {
        right = middle;
      }
    }
    return left;
  }

  void is_correct_plus() {
    int carry = 0;
    for (size_t i = 0; i < digits.size(); ++i) {
      digits[i] += carry;
      carry = digits[i] / max_digit;
      digits[i] -= 1ll * max_digit * carry;
    }
    if (carry != 0) {
      digits.push_back(0);
      digits.back() = carry;
    }
  }
  void is_correct_minus() {
    int carry = 0;
    for (size_t i = 0; i < digits.size(); ++i) {
      digits[i] -= carry;
      if (digits[i] < 0) {
        carry = (-digits[i]) / max_digit + 1;
        digits[i] += 1ll * carry * max_digit;
      } else {
        carry = 0;
      }
    }
    while (digits.size() > 1 and digits[digits.size() - 1] == 0) {
      digits.pop_back();
    }
  }

  void norm_zero() {
    if (digits.size() == 1 and module() == 0) {
      is_positive = true;
    }
  }

  void without_zero() {
    while (!digits.empty() and digits.back() == 0) {
      digits.pop_back();
    }
    if (digits.empty()) {
      digits.push_back(0);
      is_positive = true;
    }
  }

  public:
  BigInteger(int64_t num) {
    if (num < 0) {
      is_positive = false;
    } else {
      is_positive = true;
    }
    if (!is_positive) {
      num *= -1;
    }
    if (num == 0) {
      return;
    }
    while (num != 0) {
      int current_digit = num % max_digit;
      digits.back() = current_digit;
      digits.push_back(0);
      num /= max_digit;
    }
    digits.pop_back();
  }

  BigInteger(const BigInteger& num) {
    is_positive = num.is_positive;
    digits.pop_back();
    for (size_t i = 0; i < num.digits.size(); ++i) {
      digits.push_back(num.digits[i]);
    }
  }

  BigInteger() {
  }

  BigInteger(std::string line) {
    std::string substr;
    std::reverse(line.begin(), line.end());
    digits.pop_back();
    if (line.back() == '-') {
      is_positive = false;
      line.pop_back();
    }
    for (int i = 0; i < static_cast<int>(line.size()); ++i) {
      substr += line[i];
      if (i % amount_of_digits == amount_of_digits - 1) {
        std::reverse(substr.begin(), substr.end());
        digits.push_back(std::stoi(substr));
        substr.clear();
      }
    }
    if (substr.empty()) {
      return;
    }
    std::reverse(substr.begin(), substr.end());
    digits.push_back(std::stoi(substr));
  }

  BigInteger& operator=(const BigInteger& num) {
    if (this == &num) {
      return *this;
    }
    clear();
    if (digits.empty()) {
      digits.push_back(0);
    }
    is_positive = num.is_positive;
    for (size_t i = 0; i < num.digits.size(); ++i) {
      digits[i] = num.digits[i];
      digits.push_back(0);
    }
    digits.pop_back();
    return *this;
  }

  explicit operator bool() const {
    return (digits.size() != 1 || digits.front() != 0);
  }

  bool operator<(const BigInteger& other) const {
    if (is_positive != other.is_positive) {
      return other.is_positive;
    }
    if (digits.size() < other.digits.size()) {
      return other.is_positive;
    }
    if (digits.size() > other.digits.size()) {
      return !other.is_positive;
    }
    for (size_t i = other.digits.size(); i > 0; --i) {
      if (digits[i - 1] < other.digits[i - 1]) {
        return other.is_positive;
      }
      if (digits[i - 1] > other.digits[i - 1]) {
        return !other.is_positive;
      }
    }
    return false;
  }

  bool operator>(const BigInteger& other) const {
    return other < *this;
  }

  bool operator<=(const BigInteger& other) const {
    return !(other < *this);
  }

  bool operator>=(const BigInteger& other) const {
    return !(*this < other);
  }

  bool operator==(const BigInteger& other) const {
    return !(*this < other) and !(other < *this);
  }

  bool operator!=(const BigInteger& other) const {
    return (*this < other) or (other < *this);
  }

  BigInteger operator-() const {
    BigInteger result = *this;
    result.is_positive = !result.is_positive;
    result.norm_zero();
    return result;
  }

  size_t length() const {
    return digits.size();
  }

  BigInteger module() const {
    BigInteger answer = *this;
    answer.is_positive = true;
    return answer;
  }

  std::string toString() const {
    std::string answer;
    if (!is_positive) {
      answer = "-";
    } else {
      answer = "";
    }
    for (int i = static_cast<int>(digits.size() - 1); i > -1; --i) {
      std::string dig = std::to_string(digits[i]);
      int len = amount_of_digits - dig.length();
      if (i == static_cast<int>(digits.size() - 1)) {
        len = 0;
      }
      std::string current_dig;
      for (int j = 0; j < len; ++j) {
        current_dig += '0';
      }
      current_dig += dig;
      answer += current_dig;
    }
    return answer;
  }

  void clear() {
    digits.clear();
    is_positive = true;
    digits.push_back(0);
  }

  BigInteger operator+=(const BigInteger& num) {
    if (num == 0) {
      return *this;
    }
    if (is_positive != num.is_positive) {
      return (*this -= (-num));
    }
    if (module() < num.module()) {
      BigInteger copy = num;
      copy += *this;
      *this = copy;
      return *this;
    }
    for (size_t i = 0; i < num.digits.size(); ++i) {
      digits[i] += num.digits[i];
    }
    is_correct_plus();
    norm_zero();
    return *this;
  }

  BigInteger operator-=(const BigInteger& num) {
    if (num == 0) {
      return *this;
    }
    if (is_positive != num.is_positive) {
      return *this += -num;
    }
    if (module() < num.module()) {
      BigInteger copy = num;
      copy -= *this;
      *this = -copy;
      return *this;
    }
    for (size_t i = 0; i < num.digits.size(); ++i) {
      digits[i] -= num.digits[i];
    }
    is_correct_minus();
    norm_zero();
    return *this;
  }

  BigInteger operator*=(const BigInteger& num) {
    if (num == 0 or *this == 0) {
      *this = 0;
      return 0;
    }
    BigInteger result;
    result.is_positive = !(is_positive xor num.is_positive);
    std::deque<int64_t> time(static_cast<int>(num.digits.size() + digits.size()));
    for (size_t i = 0; i < num.digits.size() + digits.size(); ++i) {
      time[i] = 0;
    }
    result.digits = time;
    for (size_t i = 0; i < num.digits.size(); ++i) {
      for (size_t j = 0; j < digits.size(); ++j) {
        result.digits[i + j] += (num.digits[i] * digits[j]);
        result.digits[i + j + 1] += (result.digits[i + j] / max_digit);
        result.digits[i + j] %= max_digit;
      }
    }
    while (!result.digits.empty() and result.digits.back() == 0) {
      result.digits.pop_back();
    }
    result.norm_zero();
    *this = result;
    return result;
  }

  BigInteger operator/=(const BigInteger& num) {
    if (num == 0) {
      throw std::logic_error("division by zero");
    }
    BigInteger module = num.module();
    int is = !(is_positive xor num.is_positive);
    BigInteger result;
    result.digits.pop_back();
    BigInteger carry;
    carry.digits.pop_back();
    for (int i = static_cast<int>(digits.size() - 1); i > -1; --i) {
      carry.digits.push_front(digits[i]);
      carry.without_zero();
      result.digits.push_front(carry.del(module));
      BigInteger copy = module;
      copy *= result.digits.front();
      carry -= copy;
    }
    *this = result;
    is_positive = is;
    without_zero();
    return *this;
  }

  BigInteger operator%=(const BigInteger& num) {
    BigInteger res = module();
    res /= num.module();
    res.is_positive = !(is_positive xor num.is_positive);
    res *= num;
    *this -= res;
    return *this;
  }

  BigInteger& operator++() {
    *this += 1;
    return *this;
  }

  BigInteger operator++(int) {
    BigInteger result = *this;
    *this += 1;
    return result;
  }

  BigInteger& operator--() {
    *this -= 1;
    return *this;
  }

  BigInteger operator--(int) {
    BigInteger result = *this;
    *this -= 1;
    return result;
  }
};

BigInteger operator+(const BigInteger& num1, const BigInteger& num2) {
  BigInteger copy = num1;
  copy += num2;
  return copy;
}

BigInteger operator-(const BigInteger& num1, const BigInteger& num2) {
  BigInteger copy = num1;
  copy -= num2;
  return copy;
}

BigInteger operator*(const BigInteger& num1, const BigInteger& num2) {
  BigInteger result = num1;
  result *= num2;
  return result;
}

BigInteger operator/(const BigInteger& num1, const BigInteger& num2) {
  BigInteger result = num1;
  result /= num2;
  return result;
}

BigInteger operator%(const BigInteger& num1, const BigInteger& num2) {
  BigInteger result = num1;
  result %= num2;
  return result;
}

std::ostream& operator<<(std::ostream& output, const BigInteger& num) {
  output << num.toString();
  return output;
}

std::istream& operator>>(std::istream& input, BigInteger& num) {
  num.clear();
  std::string line;
  input >> line;
  num = BigInteger(line);
  return input;
}

BigInteger operator"" _bi(unsigned long long num) {
  BigInteger result(num);
  return result;
}

class Rational {
  private:
  BigInteger numerator = 0;
  BigInteger denominator = 1;
  bool is_positive = 1;

  BigInteger BCD(const BigInteger& num1, const BigInteger& num2) {
    if (num2 == 0) {
      return num1;
    }
    BigInteger res = num1 % num2;
    return BCD(num2, res);
  }

  public:
  Rational(const BigInteger& num) {
    if (num < 0) {
      is_positive = false;
      numerator = -num;
      return;
    }
    numerator = num;
  }

  Rational(const int& num) {
    if (num < 0) {
      is_positive = false;
      numerator = -num;
      return;
    }
    numerator = num;
  }

  Rational() {
  }

  ~Rational() {
  }

  void normalize() {
    BigInteger divider = BCD(numerator, denominator);
    numerator /= divider;
    denominator /= divider;
  }

  Rational operator-() const {
    Rational copy = *this;
    copy.is_positive = !is_positive;
    return copy;
  }

  Rational& operator+=(const Rational& num) {
    if (num.numerator == 0) {
      return *this;
    }
    if (is_positive != num.is_positive) {
      return *this -= (-num);
    }
    numerator = numerator * num.denominator + denominator * num.numerator;
    denominator = denominator * num.denominator;
    return *this;
  }

  Rational& operator-=(const Rational& num) {
    if (num.numerator == 0) {
      return *this;
    }
    if (is_positive != num.is_positive) {
      *this += (-num);
      return *this;
    }
    numerator = numerator * num.denominator - denominator * num.numerator;
    if (numerator < 0) {
      numerator = -numerator;
      is_positive = !is_positive;
    }
    denominator = denominator * num.denominator;
    return *this;
  }

  Rational& operator/=(const Rational& num) {
    numerator *= num.denominator;
    denominator *= num.numerator;
    is_positive = !(is_positive xor num.is_positive);
    if (numerator == 0) {
      is_positive = true;
    }
    return *this;
  }

  Rational& operator*=(const Rational& num) {
    numerator = numerator * num.numerator;
    denominator = denominator * num.denominator;
    is_positive = !(is_positive xor num.is_positive);
    return *this;
  }

  bool operator<(const Rational& num) const {
    if (is_positive != num.is_positive) {
      return num.is_positive;
    }
    return (numerator * num.denominator < denominator * num.numerator xor (!is_positive));
  }

  bool operator>(const Rational& num) const {
    return num < *this;
  }

  bool operator<=(const Rational& num) const {
    return !(num < *this);
  }

  bool operator>=(const Rational& num) const {
    return !(*this < num);
  }

  bool operator==(const Rational& num) const {
    return !(*this < num) && !(num < *this);
  }

  bool operator!=(const Rational& num) const {
    return (*this < num) || (num < *this);
  }

  std::string toString() const {
    Rational copy = *this;
    std::string answer;
    if (!is_positive and numerator != 0) {
      answer += '-';
    }
    copy.normalize();
    answer += copy.numerator.toString();
    if (copy.denominator == 1) {
      return answer;
    }
    answer += '/';
    answer += copy.denominator.toString();
    return answer;
  }

  std::string asDecimal(const int64_t precision = 0) const {
    BigInteger result = numerator;
    bool def = is_positive;
    for (int i = 0; i < precision; ++i) {
      result *= 10;
    }
    result /= denominator;
    std::string copied = result.toString();
    std::string answer;
    int index = 0;
    int length = static_cast<int>(copied.size());
    if (!def and numerator != 0) {
      answer.push_back('-');
    }
    if (precision >= length) {
      answer.push_back('0');
      answer.push_back('.');
    }
    for (int i = 0; i < precision - length + index; ++i) {
      answer.push_back('0');
    }
    bool flag = false;
    while (length - index > precision) {
      flag = true;
      answer.push_back(copied[index]);
      index++;
    }
    if (flag) {
      answer.push_back('.');
    }
    while (index < length) {
      answer.push_back(copied[index]);
      index++;
    }
    return answer;
  }

  explicit operator double() const {
    double result = std::stod(asDecimal(15));
    return result;
  }
};

Rational operator*(const Rational& num1, const Rational& num2) {
  Rational copy = num1;
  copy *= num2;
  return copy;
}

Rational operator/(const Rational& num1, const Rational& num2) {
  Rational copy = num1;
  copy /= num2;
  return copy;
}

Rational operator+(const Rational& num1, const Rational& num2) {
  Rational copy = num1;
  copy += num2;
  return copy;
}

Rational operator-(const Rational& num1, const Rational& num2) {
  Rational copy = num1;
  copy -= num2;
  return copy;
}
