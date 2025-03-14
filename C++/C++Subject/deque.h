#include <cassert>
#include <cstring>

// итоговый дек, все ML пофиксил, все разыменования оптимизировал

template <typename T>
class Deque {
 public:
  static constexpr inline int64_t internal_capacity = 15;

 private:
  template <bool isConstIterator>
  class basic_iterator {
   private:
    T** ptr_on_bucket_;  // указатель на ptr_on_bucket_ с этим элементом
    int64_t index_;      // индекс в бакете
    mutable T* bucket_;  // указатель на начало бакета
    mutable bool
        pointers_are_synced;  // синхронизированы ли внутренние указатели
    // mutable чтоб не копипастить для const
    // вообще эти 2 новых поля нужны чтоб оптимизировать количество
    // разыменований
    friend basic_iterator<false>;
    friend Deque<T>;

    void SyncInternalPointers() const {
      if (pointers_are_synced) {
        return;
      }
      if (ptr_on_bucket_ == nullptr) {
        bucket_ = nullptr;
      } else {
        bucket_ = *ptr_on_bucket_;
      }
      pointers_are_synced = true;
    }

   public:
    using value_type = T;
    using size_type = int64_t;
    using difference_type = int64_t;
    using reference =
        typename std::conditional_t<isConstIterator, const T&, T&>;
    using pointer = typename std::conditional_t<isConstIterator, const T*, T*>;

    basic_iterator()
        : ptr_on_bucket_(nullptr),
          index_(0),
          bucket_(nullptr),
          pointers_are_synced(true){};
    basic_iterator(T** arr, int64_t ind)
        : ptr_on_bucket_(arr),
          index_(ind),
          bucket_(*arr),
          pointers_are_synced(true) {}
    basic_iterator(const basic_iterator<false>& other)
        : ptr_on_bucket_(other.ptr_on_bucket_),
          index_(other.index_),
          bucket_(other.bucket_),
          pointers_are_synced(other.pointers_are_synced) {}

    basic_iterator& operator=(const basic_iterator<false>& other) {
      ptr_on_bucket_ = other.ptr_on_bucket_;
      index_ = other.index_;
      bucket_ = other.bucket_;
      pointers_are_synced = other.pointers_are_synced;
      return *this;
    }

    basic_iterator& operator+=(int64_t num) noexcept {
      index_ += num;
      int64_t new_index = ((index_ % internal_capacity) + internal_capacity) %
                          internal_capacity;
      ptr_on_bucket_ += (index_ - new_index) / internal_capacity;
      if ((index_ - new_index) / internal_capacity != 0) {
        pointers_are_synced = false;
      }
      index_ = new_index;
      return *this;
    }
    basic_iterator& operator-=(int64_t num) noexcept {
      return operator+=(-num);
    }
    basic_iterator operator+(int64_t num) const noexcept {
      basic_iterator copy = *this;
      copy += num;
      return copy;
    }
    basic_iterator operator-(int64_t num) const noexcept {
      basic_iterator copy = *this;
      copy -= num;
      return copy;
    }
    basic_iterator& operator++() noexcept {  // префиксный
      return operator+=(1);
    }
    basic_iterator& operator--() noexcept {  // префиксный
      return operator-=(1);
    }
    basic_iterator operator++(int) noexcept {  // постфиксный
      basic_iterator ans(*this);
      operator+=(1);
      return ans;
    }
    basic_iterator operator--(int) noexcept {  // постфиксный
      basic_iterator ans(*this);
      operator-=(1);
      return ans;
    }

    int64_t operator-(const basic_iterator& other) const noexcept {
      return (ptr_on_bucket_ - other.ptr_on_bucket_) * internal_capacity +
             (index_ - other.index_);
    }

    friend basic_iterator operator+(int64_t n, const basic_iterator& any) {
      return (any + n);
    }
    bool operator<(const basic_iterator& other) const noexcept {
      return (*this - other) < 0ll;
    }
    bool operator>=(const basic_iterator& other) const noexcept {
      return !(*this < other);
    }
    bool operator>(const basic_iterator& other) const noexcept {
      return (other < *this);
    }
    bool operator<=(const basic_iterator& other) const noexcept {
      return !(other < *this);
    }
    bool operator==(const basic_iterator& other) const noexcept {
      return (ptr_on_bucket_ == other.ptr_on_bucket_) &&
             (index_ == other.index_);
    }
    bool operator!=(const basic_iterator& other) const noexcept {
      return !operator==(other);
    }

    void CheckGranBeforeGivAns() const {
      if (index_ < 0  index_ >= internal_capacity) {
        throw std::logic_error("index_ = " + std::to_string(index_) +
                               "в итераторе, не норм");
      }
      if (ptr_on_bucket_ == nullptr) {
        throw std::logic_error(
            "разыменование итератора с ptr_on_bucket_ == nullptr");
      }
    }

    reference operator*() const {
      CheckGranBeforeGivAns();
      SyncInternalPointers();
      return bucket_[index_];
    }
    reference operator[](int64_t delta) const { return *(*this + delta); }
    pointer operator->() const {
      CheckGranBeforeGivAns();
      SyncInternalPointers();
      return (bucket_ + index_);
    }
  };

 public:
  using iterator = basic_iterator<false>;
  using const_iterator = basic_iterator<true>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

 private:
  int64_t out_cap_;
  T** out_arr_;
  iterator begin_;
  iterator end_;

  void AllocateBucket(T*& ptr) const {
    static_assert(
        std::is_same_v<decltype(new char[internal_capacity * sizeof(T)]),
                       char*>);
    ptr = reinterpret_cast<T*>(new char[internal_capacity * sizeof(T)]);
  }

  void AllocateFromTo(
      T** from,
      T** to) const {  // to INCLUSIVE!!! в случае exception-а он обратно ставит
    // nullptr-ы где аллоцировал + пробрасывает то что поймал
    for (T** cur = from; cur <= to; ++cur) {
      if (cur[0] != nullptr) {
        throw std::logic_error("в AllocateFromToIfNullptr попался не nullptr");
      }
      try {
        AllocateBucket(cur[0]);
      } catch (...) {
        cur[0] = nullptr;
        cur -= 1;
        while (cur >= from) {
          delete[] cur[0];
          cur[0] = nullptr;
          cur -= 1;
        }
        throw;
      }
    }
  }
  mutable int64_t debug_cnt = 0;
  void DeallocateFromTo(T** from, T** to) const {
    assert(from != nullptr && to != nullptr);
    assert(from >= out_arr_ && from < out_arr_ + out_cap_);
    assert(to >= out_arr_ && to < out_arr_ + out_cap_);
    assert(from <= to);
    for (T** cur = from; cur <= to; ++cur) {
      assert(cur != nullptr);
      if (*cur != nullptr) {
        debug_cnt += 1;
        delete[] reinterpret_cast<char*>(cur[0]);
        *cur = nullptr;
      }
    }
  }

  static void swap(Deque<T>& deque1, Deque<T>& deque2) {
    std::swap(deque1.out_cap_, deque2.out_cap_);
    std::swap(deque1.out_arr_, deque2.out_arr_);
    std::swap(deque1.begin_, deque2.begin_);
    std::swap(deque1.end_, deque2.end_);
  }

  void Realocate() {
    int64_t new_out_cap = 3 * out_cap_;
    T** new_out_arr;
    try {
      new_out_arr = new T*[new_out_cap];
    } catch (...) {
      throw;
    }
    static_assert(std::is_same_v<T**, decltype(new_out_arr)>);
    std::fill(new_out_arr, new_out_arr + out_cap_, nullptr);
    std::copy(out_arr_, out_arr_ + out_cap_, new_out_arr + out_cap_);
    std::fill(new_out_arr + 2 * out_cap_, new_out_arr + 3 * out_cap_, nullptr);
    iterator tmp(new_out_arr + out_cap_, 0);
    int64_t delta_begin = (begin_ - iterator(out_arr_, 0));
    iterator new_begin = tmp + delta_begin;
    iterator new_end = new_begin + (end_ - begin_);
    begin_ = new_begin;
    end_ = new_end;
    out_cap_ = new_out_cap;
    delete[] out_arr_;
    out_arr_ = new_out_arr;
  }

  void DestructFromTo(const iterator& from, const iterator& to)
      const {  // вызывает деструкторы на отрезке [from;to] (всё по
               // итераторам)
    if (from > to) {
      return;
    }
    for (iterator cur = from; cur <= to; ++cur) {
      (cur.ptr_on_bucket_[0] + cur.index_)->~T();
    }
  }

  template <bool PointerIsNullptr>
    requires(std::is_default_constructible_v<T>  !PointerIsNullptr)
  void ConstructDequeNotInConstructor(size_t sz, const T* value_ptr) {
    out_cap_ = (static_cast<int64_t>(sz) / internal_capacity) + 2;
    out_arr_ = new T*[out_cap_];
    std::fill(
        out_arr_, out_arr_ + out_cap_,
        nullptr);  // изначально так, чтоб в AllocateFromTo переходили nullptr-ы
    AllocateFromTo(out_arr_, out_arr_ + out_cap_ - 1);
    begin_ = iterator(out_arr_, 0);
    end_ = begin_ + sz;
    for (iterator cur_iter = begin_; cur_iter != end_; ++cur_iter) {
      if constexpr (!PointerIsNullptr) {
        assert(value_ptr != nullptr);
        try {
          new (cur_iter.ptr_on_bucket_[0] + cur_iter.index_) T(*value_ptr);
        } catch (...) {
          DestructFromTo(begin_, cur_iter - 1);
          DeallocateFromTo(out_arr_, out_arr_ + out_cap_ - 1);
          delete[] out_arr_;  // этого не было после 1 правки, поэтому тут был
                              // ML
          end_ = begin_;
          throw;
        }
      } else {
        assert(value_ptr == nullptr);
        try {
          new (cur_iter.ptr_on_bucket_[0] + cur_iter.index_) T();
        } catch (...) {
          DestructFromTo(begin_, cur_iter - 1);
          DeallocateFromTo(out_arr_, out_arr_ + out_cap_ - 1);
          delete[] out_arr_;  // этого не было, был memory_leak
          end_ = begin_;
          throw;
        }
      }
    }
  }

 public:
  Deque() : out_cap_(1), out_arr_(new T*[1]) {
    out_arr_[0] = reinterpret_cast<T*>(new char[sizeof(T) * internal_capacity]);
    begin_ = iterator(out_arr_, internal_capacity / 2);
    end_ = iterator(out_arr_, internal_capacity / 2);
  }

  Deque(size_t sz, const T& value) {
    ConstructDequeNotInConstructor<false>(sz, &value);
  }

  Deque(size_t sz) { ConstructDequeNotInConstructor<true>(sz, nullptr); }

  Deque(const Deque<T>& other) : out_cap_(other.out_cap_) {
    out_arr_ = new T*[out_cap_];
    std::fill(
        out_arr_, out_arr_ + out_cap_,
        nullptr);  // изначально так, чтоб в AllocateFromTo переходили nullptr-ы
    AllocateFromTo(out_arr_, out_arr_ + out_cap_ - 1);
    begin_ = iterator(out_arr_, 0);
    end_ = begin_ + other.size();
    iterator our_cur = begin_;
    iterator other_cur = other.begin_;
    while (other_cur != other.end_) {
      try {
        new (our_cur.ptr_on_bucket_[0] + our_cur.index_) T(*other_cur);
      } catch (...) {
        DestructFromTo(begin_, our_cur - 1);
        DeallocateFromTo(out_arr_, out_arr_ + out_cap_ - 1);
        delete[] out_arr_;  // этого не было после 1 ревью, так что был ML
        end_ = begin_;
        throw;
      }
      ++our_cur;
      ++other_cur;
    }
  }

  Deque& operator=(const Deque<T>& other) {
    Deque<T> copy(other);
    swap(*this, copy);
    return (*this);
  }

  T& operator[](size_t ind) { return *(begin_ + ind); }

  const T& operator[](size_t ind) const { return *(begin_ + ind); }

  T& at(size_t ind) {
    if (ind >= size()) {
      throw std::out_of_range("выход за границы массива Deque");
    }
    return operator[](ind);
  }

  const T& at(size_t ind) const {
    if (ind >= size()) {
      throw std::out_of_range("выход за границы массива Deque");
    }
    return operator[](ind);
  }

  void push_back(const T& value) {
    if (end_.ptr_on_bucket_ >= out_arr_ &&
        end_.ptr_on_bucket_ < out_arr_ + out_cap_ &&
        (end_.ptr_on_bucket_[0] == nullptr) && (end_.index_ != 0)) {
      throw std::logic_error(
          "в push_back я увидел что end_.ptr_on_bucket_ == nullptr && "
          "end_.index_ != "
          "0");
    }
    if (end_.ptr_on_bucket_ - out_arr_ == out_cap_) {
      Realocate();
      push_back(value);
      return;
    }
    if (end_.ptr_on_bucket_ - out_arr_ > out_cap_) {
      throw std::logic_error(
          "в push_back я заметил, что end_.ptr_on_bucket_ - out_arr_[0] > "
          "out_cap_, "
          "хотя "
          "должен быть инвариант что <=, и вышло, что end_.ptr_on_bucket_ - "
          "out_arr_[0] "
          "= " +
          std::to_string(end_.ptr_on_bucket_ - out_arr_) +
          " , а out_cap_ = " + std::to_string(out_cap_));
    }

    if (end_.ptr_on_bucket_[0] == nullptr) {
      try {
        AllocateBucket(end_.ptr_on_bucket_[0]);
      } catch (...) {
        end_.ptr_on_bucket_[0] = nullptr;
        throw;
      }
    }

    new (end_.ptr_on_bucket_[0] + end_.index_) T(value);
    ++end_;
  }

  void pop_back() {
    if (empty()) {
      throw std::runtime_error("нельзя pop_back-ать пустой Deque");
    }
    iterator last = end_ - 1;
    (last.ptr_on_bucket_[0] + last.index_)->~T();
    end_ -= 1;
  }

  void push_front(const T& value) {
    iterator before_begin = begin_ - 1;

    if (before_begin.ptr_on_bucket_ >= out_arr_ &&
        before_begin.ptr_on_bucket_ < out_arr_ + out_cap_ &&
        before_begin.ptr_on_bucket_[0] == nullptr &&
        before_begin.index_ != internal_capacity - 1) {
      throw std::logic_error(
          "в push_front я заметил, что before_begin.ptr_on_bucket_[0] == "
          "nullptr && "
          "before_begin.index_ != nullptr");
    }

    if (before_begin.ptr_on_bucket_ + 1 < out_arr_) {
      throw std::logic_error(
          "в push_front-е я заметил, что before_begin.ptr_on_bucket_ + 1 < "
          "out_arr_, "
          "хотя такого быть не должно");
    }

    if (before_begin.ptr_on_bucket_ + 1 == out_arr_ &&
        before_begin.index_ != internal_capacity - 1) {
      throw std::logic_error(
          "в push_front-е я заметил, что before_begin.ptr_on_bucket_ + 1 == "
          "out_arr_[0] "
          "&& before_begin.index_ != internal_capacity - 1, хотя такого быть "
          "не должно. Оказалось, что before_begin.index_ = " +
          std::to_string(before_begin.index_));
    }

    if (before_begin.ptr_on_bucket_ < out_arr_) {
      Realocate();
      push_front(value);
      return;
    }

    if (before_begin.ptr_on_bucket_[0] == nullptr) {
      try {
        AllocateBucket(before_begin.ptr_on_bucket_[0]);
      } catch (...) {
        before_begin.ptr_on_bucket_[0] = nullptr;
        throw;
      }
    }

    new (before_begin.ptr_on_bucket_[0] + before_begin.index_) T(value);
    begin_ = before_begin;
  }

  void pop_front() {
    if (empty()) {
      throw std::runtime_error("нельзя pop_front-ать пустой Deque");
    }
    (begin_.ptr_on_bucket_[0] + begin_.index_)->~T();
    ++begin_;
  }

  iterator begin() { return begin_; }

  const_iterator begin() const { return const_iterator(begin_); }

  const_iterator cbegin() const { return const_iterator(begin_); }

  reverse_iterator rbegin() { return reverse_iterator(end()); }

  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }

  const_reverse_iterator crbegin() const {
    return const_reverse_iterator(end());
  }

  iterator end() { return end_; }

  const_iterator end() const { return const_iterator(end_); }

  const_iterator cend() const { return const_iterator(end_); }

  reverse_iterator rend() { return reverse_iterator(begin()); }

  const_reverse_iterator rend() const {
    return const_reverse_iterator(begin());
  }

  const_reverse_iterator crend() const {
    return const_reverse_iterator(begin());
  }

  size_t size() const {
    if (end_ - begin_ < 0) {
      throw std::logic_error("end_ - begin_ < 0 в size-е");
    }
    return end_ - begin_;
  }

  bool empty() const { return (size() == 0); }
  void insert(const iterator& iter, const T& value) {
    if (iter > end_) {
      throw std::logic_error(
          "в insert пришёл iter > end_, кринж. iter - end = " +
          std::to_string(iter - end_));
    }
    if (iter < begin_) {
      throw std::logic_error(
          "в insert пришёл iter < begin_, кринж. iter - begin_ = " +
          std::to_string(iter - begin_));
    }
    if (iter == end_) {
      push_back(value);
      return;
    }
    T val_saved = *iter;
    *iter = value;
    insert(iter + 1, val_saved);
  }

  void erase(const iterator& iter) {
    if (iter >= end_) {
      throw std::logic_error(
          "в erase я заметил, что iter >= end_, iter - end_ = " +
          std::to_string(iter - end_));
    }
    if (iter < begin_) {
      throw std::logic_error(
          "в erase я заметил, что iter < begin_, iter - begin_ = " +
          std::to_string(iter - begin_));
    }
    iterator cur = iter;
    iterator next = iter + 1;
    while (cur + 1 < end_) {
      std::swap(*cur, *next);
      ++cur;
      ++next;
    }
    pop_back();
  }

  ~Deque() {
    assert(out_arr_ != nullptr);
    DestructFromTo(begin_, end_ - 1);
    DeallocateFromTo(out_arr_, out_arr_ + out_cap_ - 1);
    delete[] out_arr_;
  }
};