#include <cstring>
#include <iostream>

void increaseSizeCommand(char*& command, int& size_command) {
  size_command *= 2;
  char* new_command = new char[size_command];
  std::memcpy(new_command, command, size_command / 2);
  delete[] command;
  command = new_command;
}

void increaseSizeString(char**& string, int& size_string) {
  size_string *= 2;
  char** new_string = new char*[size_string];
  std::memcpy(new_string, string, size_string / 2 * sizeof(char*));
  delete[] string;
  string = new_string;
}

void readCommand(char*& command, int& length, int& size_command) {
  char symbol;
  while (std::cin.get(symbol)) {
    if (length == size_command - 1) {
      increaseSizeCommand(command, size_command);
    }
    if (symbol == '\n') {
      command[length] = '\0';
      ++length;
      break;
    }
    command[length] = symbol;
    ++length;
  }
}

void push(char**& string, int& amount, int& size_string, char*& command,
          int& length) {
  if (amount == size_string - 1) {
    increaseSizeString(string, size_string);
  }
  char* line = new char[length - 5];
  for (int i = 0; i < length - 5; ++i) {
    line[i] = command[i + 5];
  }
  string[amount] = line;
  delete[] command;
  ++amount;
  std::cout << "ok" << '\n';
}

void pop(char** string, int& amount, char*& command) {
  if (amount == 0) {
    std::cout << "error" << '\n';
    delete[] command;
    return;
  }
  --amount;
  int i = 0;
  while (string[amount][i] != '\0') {
    std::cout << string[amount][i];
    ++i;
  }
  std::cout << '\n';
  delete[] string[amount];
  delete[] command;
}

void back(char** string, int& amount, char*& command) {
  if (amount == 0) {
    std::cout << "error" << '\n';
    delete[] command;
    return;
  }
  int i = 0;
  while (string[amount - 1][i] != '\0') {
    std::cout << string[amount - 1][i];
    ++i;
  }
  std::cout << '\n';
  delete[] command;
}

void len(int amount, char*& command) {
  std::cout << amount;
  delete[] command;
  std::cout << '\n';
}

void clear(char**& string, int& amount, int& size_string, char*& command) {
  for (int i = 0; i < amount; ++i) {
    delete[] string[i];
  }
  delete[] string;
  amount = 0;
  size_string = 2;
  string = new char*[size_string];
  delete[] command;
  std::cout << "ok" << '\n';
}

void exit(char**& string, int& amount, char*& command) {
  for (int i = 0; i < amount; ++i) {
    delete[] string[i];
  }
  delete[] string;
  delete[] command;
  std::cout << "bye" << '\n';
}

int main() {
  int amount = 0;
  int size_string = 2;
  char** string = new char*[size_string];
  while (true) {
    int length = 0;
    int size_command = 2;
    char* command = new char[size_command];
    readCommand(command, length, size_command);
    if (strcmp("pop", command) == 0) {
      pop(string, amount, command);
    } else if (strcmp("back", command) == 0) {
      back(string, amount, command);
    } else if (strcmp("size", command) == 0) {
      len(amount, command);
    } else if (strcmp("clear", command) == 0) {
      clear(string, amount, size_string, command);
    } else if (strcmp("exit", command) == 0) {
      exit(string, amount, command);
      break;
    } else {
      push(string, amount, size_string, command, length);
    }
  }
  return 0;
}
