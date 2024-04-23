#include <cmath>
#include <iostream>
#include <string>
#include <vector>

class Heap {
  private:
  std::vector<std::pair<long long, long long>> heap_;
  std::vector<long long> positions_;

  static long long Father(long long son) { return (son - 1) / 2; }
  static long long LeftSon(long long father) { return 2 * father + 1; }
  static long long RightSon(long long father) { return 2 * father + 2; }
  void siftUp(long long current_index) {
    while (current_index != 0 &&
           heap_[Father(current_index)] > heap_[current_index]) {
      std::swap(heap_[current_index], heap_[Father(current_index)]);
      std::swap(positions_[heap_[current_index].second],
                positions_[heap_[Father(current_index)].second]);
      current_index = Father(current_index);
    }
  }

  void siftDown(int index) {
    long long swap = -1;
    if (RightSon(index) <= static_cast<long long>(heap_.size()) &&
        heap_[index].first > heap_[LeftSon(index)].first) {
      swap = LeftSon(index);
    }
    if (RightSon(index) < static_cast<long long>(heap_.size()) &&
        heap_[index].first > heap_[RightSon(index)].first &&
        heap_[LeftSon(index)].first > heap_[RightSon(index)].first) {
      swap = RightSon(index);
    }
    while (swap > -1) {
      std::swap(heap_[index], heap_[swap]);
      std::swap(positions_[heap_[index].second],
                positions_[heap_[swap].second]);
      index = swap;
      swap = -1;
      if (RightSon(index) <= static_cast<long long>(heap_.size()) &&
          heap_[index].first > heap_[LeftSon(index)].first) {
        swap = LeftSon(index);
      }
      if (RightSon(index) < static_cast<long long>(heap_.size()) &&
          heap_[index].first > heap_[RightSon(index)].first &&
          heap_[LeftSon(index)].first > heap_[RightSon(index)].first) {
        swap = RightSon(index);
      }
    }
  }
  public:
  Heap(int amount) { positions_.resize(amount); }
  void Insert(std::pair<long long, long long> number) {
    heap_.push_back(number);
    positions_[number.second] = static_cast<long long>(heap_.size()) - 1;
    siftUp(static_cast<long long>(heap_.size()) - 1);
  }

  long long GetMin() { return heap_[0].first; }

  void ExtractMin() {
    heap_[0] = heap_[static_cast<long long>(heap_.size()) - 1];
    heap_.pop_back();
    siftDown(0);
  }

  void DecreaseKey(long long index, long long change) {
    heap_[positions_[index]].first -= change;
    siftUp(positions_[index]);
  }
};

int main() {
  long long digit;
  long long amount;
  std::string operation;
  std::cin >> amount;
  Heap heap = Heap(amount);
  for (long long i = 1; i <= amount; ++i) {
    std::cin >> operation;
    if (operation == "insert") {
      std::cin >> digit;
      std::pair<long long, long long> number{digit, i};
      heap.Insert(number);
    }
    if (operation == "getMin") {
      std::cout << heap.GetMin();
    }
    if (operation == "extractMin") {
      heap.ExtractMin();
    }
    if (operation == "decreaseKey") {
      long long index;
      long long change;
      std::cin >> index >> change;
      heap.DecreaseKey(index, change);
    }
  }
  return 0;
}