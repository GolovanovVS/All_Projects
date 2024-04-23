#include <cstring>
#include <iostream>

class String {
  private:
  size_t length_ = 0;
  size_t capacity_ = 1;
  char* line_ = new char[0];

  void decreaseCapacity(int number) {
    capacity_ += number;
    char* new_line_ = new char[capacity_];
    std::memcpy(new_line_, line_, (capacity_ - number) * sizeof(char));
    delete[] line_;
    line_ = new_line_;
  }

  bool stringsAreSame(const String& string, int index) const {
    for (int i = 0; i < static_cast<int>(string.length_); ++i) {
      if (line_[i + index] != string.line_[i]) {
        return false;
      }
    }
    return true;
  }

  public:
  String(const char* string)
      : length_(std::strlen(string)),
        capacity_(std::strlen(string) + 1),
        line_(new char[capacity_]) {
    std::memcpy(line_, string, capacity_);
    line_[capacity_ - 1] = '\0';
  }

  String(int number, char symbol)
      : length_(number),
        capacity_(number + 1),
        line_(new char[capacity_]) {
    std::fill(line_, line_ + number, symbol);
    line_[capacity_ - 1] = '\0';
  }

  String(const String& string)
      : length_(string.length_),
        capacity_(string.capacity_),
        line_(new char[capacity_]) {
    std::strcpy(line_, string.line_);
  }

  String() {
    line_[0] = '\0';
  }

  ~String() {
    delete[] line_;
  }

  String& operator=(const String& string) {
    if (this != &string) {
      delete[] line_;
      length_ = string.length_;
      capacity_ = string.capacity_;
      line_ = new char[capacity_];
      std::strcpy(line_, string.line_);
    }
    return *this;
  }

  size_t size() const {
    return length_;
  }

  char& operator[](int index) {
    return line_[index];
  }

  const char& operator[](int index) const {
    return line_[index];
  }

  String& operator+=(const String& string) {
    if (length_ + string.length_ >= capacity_) {
      decreaseCapacity(std::max(length_ + string.length_ - capacity_ + 1, capacity_));
    }
    std::strcat(line_, string.line_);
    length_ += string.length_;
    return *this;
  }

  String& operator+=(char symbol) {
    push_back(symbol);
    return *this;
  }

  String operator+(char symbol) {
    String result = *this;
    result.push_back(symbol);
    return result;
  }

  size_t find(const String& string) const {
    for (int i = 0; i <= static_cast<int>(length_ - string.length_); ++i) {
      if (stringsAreSame(string, i)) {
        return i;
      }
    }
    return length_;
  }

  size_t rfind(const String& string) const {
    for (int i = static_cast<int>(length_ - string.length_); i > -1; --i) {
      if (stringsAreSame(string, i)) {
        return i;
      }
    }
    return length_;
  }

  String substr(int start, int count) const {
    int real_size = std::min(static_cast<int>(length_) - start, count);
    String answer = String(real_size, '0');
    for (int i = 0; i < count; ++i) {
      if (i == static_cast<int>(length_)) {
        break;
      }
      answer[i] = line_[start + i];
    }
    return answer;
  }

  bool empty() {
    return length_ == 0;
  }

  void clear() {
    length_ = 0;
    line_[0] = '\0';
  }

  void shrink_to_fit() {
    char* new_line_ = new char[length_ + 1];
    std::strcpy(new_line_, line_);
    delete[] line_;
    line_ = new_line_;
    capacity_ = length_ + 1;
  }

  size_t length() const {
    return length_;
  }

  size_t capacity() const {
    return capacity_ - 1;
  }

  void push_back(char symbol) {
    if (length_ == capacity_ - 1) {
      decreaseCapacity(capacity_);
    }
    line_[length_] = symbol;
    line_[length_ + 1] = '\0';
    length_ += 1;
  }

  void pop_back() {
    line_[length_ - 1] = '\0';
    length_ -= 1;
  }

  char& front() {
    return line_[0];
  }

  const char& front() const {
    return line_[0];
  }

  char& back() {
    return line_[length_ - 1];
  }

  const char& back() const {
    return line_[length_ - 1];
  }

  char* data() {
    return line_;
  }

  const char* data() const {
    return line_;
  }
};

bool operator<(const String& str1, const String& str2) {
  return memcmp(str1.data(), str2.data(), std::min(str1.size(), str2.size()) + 1) < 0;
}

bool operator>(const String& str1, const String& str2) {
  return str2 < str1;
}

bool operator>=(const String& str1, const String& str2) {
  return !(str1 < str2);
}

bool operator<=(const String& str1, const String& str2) {
  return !(str2 < str1);
}

bool operator==(const String& str1, const String& str2) {
  return !(str2 < str1) && !(str1 < str2);
}

bool operator!=(const String& str1, const String& str2) {
  return (str2 < str1) || (str1 < str2);
}

std::ostream& operator<<(std::ostream& output, const String& str) {
  output << str.data();
  return output;
}

std::istream& operator>>(std::istream& input, String& str) {
  str.clear();
  char symbol = input.get();
  while (symbol == ' ' || symbol == '\n') {
    symbol = input.get();
  }
  while (symbol != ' ' && symbol != '\n' && !input.eof()) {
    str.push_back(symbol);
    symbol = input.get();
  }
  return input;
}

String operator+(char symbol, const String& str) {
  String result;
  result.push_back(symbol);
  result += str;
  return result;
}

String operator+(const String& str1, const String& str2) {
  String result = str1;
  result += str2;
  return result;
}