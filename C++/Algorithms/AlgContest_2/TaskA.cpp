#include <iostream>
#include <string>
#include <vector>

bool LineIsCorrect(std::string& line) {
  std::vector<char> brackets;
  int number = -1;
  for (auto& element : line) {
    if (element == '(' or element == '{' or element == '[') {
      brackets.push_back(element);
      number++;
    }
    if (number == -1) {
      return false;
    }
    if (element == ')') {
      if (brackets[number] == '(') {
        brackets.pop_back();
        number--;
      } else {
        return false;
      }
    }
    if (element == '}') {
      if (brackets[number] == '{') {
        brackets.pop_back();
        number--;
      } else {
        return false;
      }
    }
    if (element == ']') {
      if (brackets[number] == '[') {
        brackets.pop_back();
        number--;
      } else {
        return false;
      }
    }
  }
  return (number == -1);
}

int main() {
  std::string line;
  std::cin >> line;
  if (LineIsCorrect(line)) {
    std::cout << "YES";
  } else {
    std::cout << "NO";
  }
  return 0;
}
