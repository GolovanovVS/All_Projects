#include <memory>

template <size_t N>
struct StackStorage {
  char storage[N];
  void* used = storage;
  size_t free = N;
  bool operator==(const StackStorage& other) {
    return this->used == other.used;
  }
  bool operator!=(const StackStorage& other) {
    return this->used != other.used;
  }
};

template <typename T, size_t N>
class StackAllocator {
 private:
  StackStorage<N>& storage_;

  template <typename U, size_t M>
  friend class StackAllocator;

 public:
  using value_type = T;
  using pointer = T*;
  template <typename U>
  StackAllocator(const StackAllocator<U, N>& other)
      : storage_(other.storage_){};

  StackAllocator() noexcept = default;
  explicit StackAllocator(StackStorage<N>& storage_other) noexcept
      : storage_(storage_other){};
  ~StackAllocator() = default;

  StackAllocator& operator=(const StackAllocator& other) {
    if (this != &other) {
      return *this;
    }
    storage_ = other.storage_;
    return *this;
  }

  pointer allocate(size_t n) {
    std::align(alignof(value_type), sizeof(value_type) * n, storage_.used,
               storage_.free);
    auto answer = reinterpret_cast<pointer>(storage_.used);
    storage_.used = reinterpret_cast<char*>(answer) + sizeof(value_type) * n;
    storage_.free -= n * sizeof(value_type);
    return answer;
  }

  void deallocate(pointer ptr, size_t n) noexcept {}
  void destroy(pointer ptr) noexcept { ptr->~value_type(); }

  template <typename U>
  struct rebind {
    using other = StackAllocator<U, N>;
  };

  template <typename U>
  bool operator==(const StackAllocator<U, N>& other) const {
    return storage_ == other.storage_;
  }

  template <typename U>
  bool operator!=(const StackAllocator<U, N>& other) const {
    return storage_ != other.storage_;
  }
};

template <typename T, typename Allocator = std::allocator<T>>
class list {
 private:
  struct BaseNode {
    BaseNode* prev;
    BaseNode* next;
  };
  struct Node : BaseNode {
    T value;
  };

  template <bool is_const>
  class BaseIterator {
   private:
    using value_type = std::conditional<is_const, const T, T>;
    using reference_type = std::conditional<is_const, const T&, T&>;
    using pointer_type = std::conditional<is_const, const T*, T*>;
    BaseNode* node;

   public:
    BaseIterator() = default;
    BaseIterator(const BaseIterator& other) : node(other.node){};
    BaseIterator(BaseNode* nod) : node(nod) {};
    BaseIterator(Node* nod) : node(nod) {};
    Node* take_node() { return node; }
    reference_type operator&() {
      //TODO : write &
    }
    pointer_type operator*() { return dynamic_cast<Node>(*this).value; }
    BaseIterator& operator++() {
      node = node->next;
      return *this;
    }
    BaseIterator& operator++(int) {
      auto ans = this;
      node = node->next;
      return *ans;
    }
    BaseIterator& operator--() {
      node = node->prev;
      return *this;
    }
    BaseIterator& operator--(int) {
      auto ans = this;
      node = node->prev;
      return *ans;
    }
    bool operator!=(const BaseIterator& other) const {
      return node != other.node;
    };
    bool operator==(const BaseIterator& other) const {
      return node == other.node;
    };
  };
  [[no_unique_address]] Allocator all = Allocator();
  template <typename allocat>
  Node* allocate_nodes(size_t n, T object, allocat& alloc) {
    Node* cur;
    Node* res;
    for (size_t i = 0; i < n; ++i) {
      Node* ans = std::allocator_traits<allocat>::allocate(alloc, 1);
      ans->value = object;
      cur = ans;
      if (i == 0) {
        res = ans;
        continue;
      }
      cur->next = ans;
      ans->prev = cur;
    }
    return res;
  }

 public:
  using value_type = T;
  using allocator_type = Allocator;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = Allocator::pointer;
  using iterator = BaseIterator<false>;
  using const_iterator = BaseIterator<true>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  BaseNode fakeNode;
  size_t sz;
  list() {
    fakeNode.next = *fakeNode;
    fakeNode.prev = *fakeNode;
    sz = 0;
  };
  list(size_t size) {
    Node* cur;
    for (size_t i = 0; i < size; ++i) {
      Node* ans = std::allocator_traits<Allocator>::allocate(all, 1);
      if (i == 0) {
        cur->prev = fakeNode;
        continue;
      }
      cur->next = ans;
      ans->prev = cur;
      cur = ans;
    }
    cur->next = fakeNode;
    sz = size;
  }
  list(size_t size, const T& val) : sz(0){
    Node* cur;
    for (size_t i = 0; i < size; ++i) {
      Node* ans = std::allocator_traits<Allocator>::allocate(all, 1);
      cur->value = val;
      if (i == 0) {
        cur->prev = fakeNode;
        continue;
      }
      cur->next = ans;
      ans->prev = cur;
      cur = ans;
    }
    cur->next = fakeNode;
    sz = size;
  }
  list(const Allocator& alloc) : fakeNode{&fakeNode, &fakeNode}, all(alloc) , sz(0) {}
  list(size_t size, const Allocator& alloc) : list(alloc) {
    Node* cur;
    for (size_t i = 0; i < size; ++i) {
      Node* ans = std::allocator_traits<Allocator>::allocate(all, 1);
      if (i == 0) {
        cur->prev = fakeNode;
        continue;
      }
      cur->next = ans;
      ans->prev = cur;
      cur = ans;
    }
    cur->next = fakeNode;
    sz = size;
  }
  list(size_t size, const T& val, const Allocator& alloc) : list(alloc) {
    Node* cur;
    for (size_t i = 0; i < size; ++i) {
      Node* ans = std::allocator_traits<Allocator>::allocate(all, 1);
      cur->value = val;
      if (i == 0) {
        cur->prev = fakeNode;
        continue;
      }
      cur->next = ans;
      ans->prev = cur;
      cur = ans;
    }
    cur->next = fakeNode;
    sz = size;
  }
  const Allocator& get_allocator() { return all; };
  list(const list& other) : list(other.get_allocator()) {
    iterator cur = other.begin();
    for (size_t i = 0; i < other.sz; ++i) {
      push_back(cur.take_node()->value);
      ++cur;
    }
  }
  list operator=(const list& other) {
    auto new_alloc = std::allocator_traits<Allocator>::propagate_on_container_copy_assignment::value
                         ? other.all
                         : all;
    size_t old = sz;
    //TODO make this huinya
  }
  iterator begin() { return iterator{fakeNode.next}; }
  const_iterator begin() const { return const_iterator{fakeNode.next}; }
  const_iterator cbegin() const { return const_iterator{fakeNode}; }
  iterator end() { return iterator{ fakeNode}; }
  const_iterator end() const { return const_iterator{fakeNode}; }
  const_iterator cend() const { return const_iterator{fakeNode}; }
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const { return reverse_iterator(end()); }
  const_reverse_iterator crbegin() const { return reverse_iterator(end()); }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const { return reverse_iterator(begin()); }
  const_reverse_iterator crend() const { return reverse_iterator(begin()); }
  void pop_back() { erase(--end()); }
  void pop_front() { erase(begin()); }
  void push_back(const T& value) {
    insert(static_cast<BaseIterator<false>>(end()), value);
  }
  void push_front(const T& value) { insert(begin(), value); }
  ~list() {
    while (sz > 0) {
      pop_back();
    }
  }
  size_t size() { return sz; }
  void insert(BaseIterator<false> iter, const value_type& object) {
    Node* res = allocate_nodes<Allocator>(1, object, Allocator());
    res->next = iter.take_node();
    iter.take_node()->prev = res;
    --iter;
    iter.take_node()->next = res;
    res->prev = iter.take_node();
    ++sz;
  }
  void erase(BaseIterator<true> iter) {
    Node* cur = iter.take_node();
    cur->prev->next = cur->next;
    cur->next->prev = cur->prev;
    std::allocator_traits<Allocator>::destroy(Allocator(),
                                              dynamic_cast<Node*>(cur));
    std::allocator_traits<Allocator>::deallocate(Allocator(),
                                                 dynamic_cast<Node*>(cur), 1);
    --sz;
  }
};