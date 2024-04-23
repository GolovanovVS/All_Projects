#include <vector>

template <typename T>
class Deque {
private:
  const int bucketSize = 16;
  T** mas;
  int begin = 0;
  int end = 1;
  int capacity = 1;
  template <bool is_const>
  class BaseIterator {
    private:
      using value_type = std::conditional<is_const, const T, T>;
      using reference_type = std::conditional<is_const, const T&, T&>;
      using pointer_type = std::conditional<is_const, const T*, T*>;
      value_type** storage;
      size_t Pos;
      explicit BaseIterator(value_type** storage, size_t Pos): storage(storage), Pos(Pos) {}

    public:
      BaseIterator(const BaseIterator& other) = default;
      template <typename Iter>
      explicit BaseIterator(const Iter& other): storage(other.storage_), Pos(other.Pos){}
      BaseIterator& operator+=(size_t pos) {
        Pos += pos;
        return *this;
      }
      bool operator<(const BaseIterator& other) const = default;
      bool operator>(const BaseIterator& other) const = default;
      bool operator<=(const BaseIterator& other) const = default;
      bool operator>=(const BaseIterator& other) const = default;
      bool operator!=(const BaseIterator& other) const = default;
      bool operator==(const BaseIterator& other) const = default;



  };

  T* createBacket(){
    char* back = new char[bucketSize * sizeof(T)];
    T* res = static_cast<T*>(back);
    return res;
  };

  void reallocate(){
    T** new_mas = new T*[3 * capacity];
    for (int i = capacity; i < 2 * capacity; ++i) {
      new_mas[i] = mas[i - capacity];
    }
    mas = new_mas;
    begin += capacity;
    end += capacity;
    capacity *= 3;

  }

public:
  Deque() {
    T* back = createBacket();
    mas = new T*{back};
  }
  Deque(int number): mas(new T*(number)){
    mas = new T*[number / bucketSize + 1];
    for (T* element : mas) {
      element = createBacket();
    }
  }
  Deque(int number, const T& el) {
    mas = new T*[number / bucketSize + 1];
    for (T* element : mas) {
      element = createBacket();
      for (int i = 0; i < std::min(bucketSize, number); ++i) {
        element[i] = el;
        --number;
      }
    }
    end = number;
  }
  ~Deque(){
    for (T* element : mas) {
      for (T el : element) {
        ~el;
      }
      char* ll = reinterpret_cast<T*> element;
    }
    delete mas;
  }

  size_t size(){
    return end - begin;
  }
};