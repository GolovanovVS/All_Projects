#include <deque>
#include <iostream>
#include <vector>

struct Answer {
  std::deque<int> first_middle;
  std::deque<int> last_middle;
  std::vector<int> result;
};

void AddPoor(Answer& turn, int name) {
  turn.last_middle.push_back(name);
  if ((int)turn.first_middle.size() != (int)turn.last_middle.size()) {
    int member = turn.last_middle.front();
    turn.last_middle.pop_front();
    turn.first_middle.push_back(member);
  }
}

void AddRich(Answer& turn, int name) {
  if ((int)turn.first_middle.size() == (int)turn.last_middle.size()) {
    turn.first_middle.push_back(name);
  } else {
    turn.last_middle.push_front(name);
  }
}

void Delete(Answer& turn) {
  turn.result.push_back(turn.first_middle.front());
  turn.first_middle.pop_front();
  if ((int)turn.first_middle.size() != (int)turn.last_middle.size()) {
    int member = turn.last_middle.front();
    turn.last_middle.pop_front();
    turn.first_middle.push_back(member);
  }
}

int main() {
  int amount;
  int number;
  std::cin >> amount;
  Answer turn;
  char operation;
  for (int i = 0; i < amount; ++i) {
    std::cin >> operation;
    if (operation == '-') {
      Delete(turn);
    } else {
      std::cin >> number;
      if (operation == '+') {
        AddPoor(turn, number);
      }
      if (operation == '*') {
        AddRich(turn, number);
      }
    }
  }
  for (auto element : turn.result) {
    std::cout << element << '\n';
  }
  return 0;
}
