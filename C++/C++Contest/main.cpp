#include <list>
#include "list.h"

int main() {
  StackStorage<100'000> storage;
  StackAllocator<int, 100'000> alloc(storage);
  std::list<int, alloc> v(alloc);
}