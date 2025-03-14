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

  void deallocate(pointer, size_t) noexcept {}
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
class List {
 private:
  struct BaseNode {
    BaseNode* prev;
    BaseNode* next;

    BaseNode() = default;
    BaseNode(BaseNode* prev, BaseNode* next) : prev(prev), next(next) {}
  };

  struct Node : public BaseNode {
    T value;

    template <typename U>
    Node(BaseNode* prev, BaseNode* next, U&& val) : BaseNode(prev, next), value(std::forward<U>(val)) {}

    Node(BaseNode* prev, BaseNode* next) : BaseNode(prev, next), value() {}
  };

  template <bool is_const>
  class BaseIterator {
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename std::conditional_t<is_const, const T, T>;
    using reference = typename std::conditional_t<is_const, const T&, T&>;
    using pointer = typename std::conditional_t<is_const, const T*, T*>;
    using difference_type = std::ptrdiff_t;
    
   private:
    BaseNode* node_;

    BaseNode* take_base_node() const {
      return node_;
    }

    Node* take_node() const {
      return static_cast<Node*>(node_);
    }

    friend class List;

   public:
    BaseIterator() = default;
    BaseIterator(const BaseIterator<false>& other) : node_(other.node_){};
    BaseIterator(BaseNode* node) : node_(node) {};
    BaseIterator(Node* node) : node_(node) {};

    BaseIterator& operator=(const BaseIterator<false>& other) {
      node_ = other.node_;
      return *this;
    }

    reference operator*() const {
      return static_cast<Node*>(node_)->value;
    }

    pointer operator->() const {
      return &(static_cast<Node*>(node_)->value);
    }

    BaseIterator& operator++() {
      node_ = node_->next;
      return *this;
    }
    BaseIterator operator++(int) {
      auto ans = this;
      node_ = node_->next;
      return *ans;
    }
    BaseIterator& operator--() {
      node_ = node_->prev;
      return *this;
    }
    BaseIterator operator--(int) {
      auto ans = this;
      node_ = node_->prev;
      return *ans;
    }

    template <bool isconst>
    bool operator!=(const BaseIterator<isconst>& other) const {
      return node_ != other.node_;
    };

    template <bool isconst>
    bool operator==(const BaseIterator<isconst>& other) const {
      return node_ == other.node_;
    };
  };

 public:
  using iterator = BaseIterator<false>;
  using const_iterator = BaseIterator<true>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  
  using node_alloc = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
  using nd_alloc_traits = std::allocator_traits<node_alloc>;


 private:
  template <typename allocat>
  Node* allocate_nodes(size_t n, T object, allocat& alloc) {
    Node* cur;
    Node* res;
    using node_alloc_all = typename std::allocator_traits<allocat>::template rebind_alloc<Node>;
    node_alloc_all alloc_ = alloc;
    for (size_t i = 0; i < n; ++i) {
      Node* ans = std::allocator_traits<node_alloc>::allocate(alloc_, 1);
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

  template <typename... Args>
  void emplace_(const_iterator it, Args&&... args) {
    node_alloc alloc = val_alloc;
    Node* new_node = nd_alloc_traits::allocate(alloc, 1);

    try {
      nd_alloc_traits::construct(
          alloc, new_node, it.take_base_node()->prev, it.take_base_node(),
          std::forward<Args>(args)...);

    } catch (...) {
      nd_alloc_traits::deallocate(alloc, new_node, 1);
      throw;
    }

    it.take_base_node()->prev->next = new_node;
    it.take_base_node()->prev = new_node;
    ++list_size;
  }
  
  List(const List& other, Allocator& alloc) : List(alloc) {
    for (auto& element : other) {
      push_back(element);
    }
  }
 
 public:
  using value_type = T;
  using allocator_type = Allocator;
  using reference = value_type&;
  using const_reference = const value_type&;

  using default_alloc = Allocator;

  mutable BaseNode fakeNode;
  size_t list_size;
  [[no_unique_address]] default_alloc val_alloc;

  List(const Allocator& alloc) : fakeNode{&fakeNode, &fakeNode}, list_size(0), val_alloc(alloc) {}

  List() : List(Allocator()) {}
  
  List(size_t size, const Allocator& alloc) : List(alloc) {    
    node_alloc node_alloc_(val_alloc);
    while (list_size < size) {
      emplace_(end());
    }
  }  

  explicit List(size_t size) : List(size, Allocator()) {}

  List(size_t size, const T& val, const Allocator& alloc) : List(alloc) {
    node_alloc node_all_ = val_alloc;
    Node* cur = nd_alloc_traits::allocate(node_all_, 1);
    cur->prev = &fakeNode;
    cur->value = val;
    for (size_t i = 1; i < size; ++i) {
      Node* ans = nd_alloc_traits::allocate(node_all_, 1);
      cur->value = val;
      cur->next = ans;
      ans->prev = cur;
      cur = ans;
    }
    cur->next = &fakeNode;
    list_size = size;
  }
  
  List(size_t size, const T& val) : List(size, val, Allocator()) {}

  const Allocator& get_allocator() const { return val_alloc; };

  List(const List& other)
      : List(std::allocator_traits<Allocator>::select_on_container_copy_construction(
                other.get_allocator())) {
    for (auto& element : other) {
      push_back(element);
    }
  }
  
  List(List&& other) : fakeNode(other.fakeNode), val_alloc(std::move(other.val_alloc)), list_size(other.list_size) {
    other.fakeNode = BaseNode(&other.fakeNode, &other.fakeNode);

    fakeNode.next->prev = &fakeNode;
    fakeNode.prev->next = &fakeNode;
  }

  List& operator=(const List& other) {
    if (this == &other) {
      return *this;
    }
    auto new_alloc = std::allocator_traits<Allocator>::propagate_on_container_copy_assignment::value
                         ? other.val_alloc
                         : val_alloc;
    List temporary(other, new_alloc);
    std::swap(fakeNode, temporary.fakeNode);
    std::swap(list_size, temporary.list_size);
    std::swap(val_alloc, temporary.val_alloc);
    
    fakeNode.prev->next = &fakeNode;
    fakeNode.next->prev = &fakeNode;

    temporary.fakeNode.prev->next = &temporary.fakeNode;
    temporary.fakeNode.next->prev = &temporary.fakeNode;
    return *this;
  }

  iterator begin() { return iterator{fakeNode.next}; }
  const_iterator begin() const { return const_iterator{fakeNode.next}; }
  const_iterator cbegin() const noexcept { return const_iterator{fakeNode.next}; }
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
  const_reverse_iterator crbegin() const { return const_reverse_iterator(end()); }
  iterator end() { return iterator{&fakeNode}; }
  const_iterator end() const { return const_iterator{&fakeNode}; }
  const_iterator cend() const { return const_iterator{&fakeNode}; }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
  const_reverse_iterator crend() const { return const_reverse_iterator(begin()); }
  void pop_back() { erase(--end()); }
  void pop_front() { erase(begin()); }

  void push_back(const T& value) { insert(end(), value); }
  void push_front(const T& value) { insert(begin(), value); }

  ~List() {
    while (list_size > 0) {
      pop_back();
    }
  }
 
  size_t size() const { return list_size; }

  template <typename... Args>
  void insert(BaseIterator<true> iter, Args&&... args) {
    node_alloc nd_all = val_alloc;
    emplace_(iter, std::forward<Args>(args)...);
  }

  void erase(BaseIterator<true> iter) {
    BaseNode* cur = iter.take_base_node();
    cur->prev->next = cur->next;
    cur->next->prev = cur->prev;
    node_alloc all_ = val_alloc;
    nd_alloc_traits::destroy(all_,
                                              static_cast<Node*>(cur));
    nd_alloc_traits::deallocate(all_,
                                                 static_cast<Node*>(cur), 1);
    --list_size;
  }
};
