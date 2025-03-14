#include <iostream>
#include "string.h"

int main() {
  String s = String(1, 'a');
  String a = String("def");
  int x = String("abc").substr(1, 10000).size();
  std::cout << (x == 2);
}

