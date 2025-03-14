#include <cstddef>
#include <functional>
#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>

template <typename U, typename T>
concept IsFamily = std::is_base_of_v<T, U> || std::is_same_v<T, U>;

template <typename T>
class SharedPtr;

template <typename T>
class WeakPtr;

class BaseControlBlock {
 private:
  virtual void delete_block() {};

 public:
  std::size_t sharedCount_ = 1;
  std::size_t weakCount_ = 0;

  void increase_shared_counter() { ++sharedCount_; }

  void increase_weak_counter() { ++weakCount_; }

  void decrease_shared_counter() { --sharedCount_; }

  void decrease_weak_counter() { --weakCount_; }

  BaseControlBlock(size_t sh_count, size_t we_count)
      : sharedCount_(sh_count), weakCount_(we_count){};

  void decreaseWeak() {
    --weakCount_;
    if (sharedCount_ == 0) {
      delete_block();
    }
  }

  virtual void decreaseShared(void*) {};

  virtual ~BaseControlBlock() = default;
};

template <typename T, typename U, typename Del = std::default_delete<U>,
          typename All = std::allocator<U>>
class ControlBlockRegular : public BaseControlBlock {
 private:
  All all_;
  Del del_;

  void delete_block() {
    if (weakCount_ == 0) {
      typename std::allocator_traits<All>::template rebind_alloc<
          ControlBlockRegular>
          block_all(all_);

      std::allocator_traits<
          typename std::allocator_traits<All>::template rebind_alloc<
              ControlBlockRegular>>::deallocate(block_all, this, 1);
    }
  }

  ControlBlockRegular(size_t sh_count, size_t we_count)
      : BaseControlBlock(sh_count, we_count) {}

  ControlBlockRegular(size_t sh_count, size_t we_count, Del del)
      : BaseControlBlock(sh_count, we_count), del_(del) {}

  ControlBlockRegular(size_t sh_count, size_t we_count, All all)
      : BaseControlBlock(sh_count, we_count), all_(all) {}

  ControlBlockRegular(size_t sh_count, size_t we_count, Del del, All all)
      : BaseControlBlock(sh_count, we_count), all_(all), del_(del) {}

 public:
  void decreaseShared(void* ptr) {
    if (--sharedCount_ != 0) {
      return;
    }
    std::invoke(del_, static_cast<U*>(reinterpret_cast<T*>(ptr)));
    delete_block();
  }

  friend class SharedPtr<T>;
  friend class WeakPtr<T>;
};

template <typename T, typename All = std::allocator<T>>
class ControlBlockMakeShared : public BaseControlBlock {
 public:
  T value_;
  All all_;

 private:
   void delete_block() {
    if (weakCount_ != 0) {
      return;
    }
    typename std::allocator_traits<All>::template rebind_alloc<
        ControlBlockMakeShared>
        object;
    std::allocator_traits<typename std::allocator_traits<
        All>::template rebind_alloc<ControlBlockMakeShared>>::deallocate(object,
                                                                         this,
                                                                         1);
  }

 public:
  template <typename... Args>
  ControlBlockMakeShared(size_t sh_count, size_t wk_count, Args&&... value)
      : BaseControlBlock(sh_count, wk_count, All(),
                         std::forward<Args>(value)...) {}

  template <typename... Args>
  ControlBlockMakeShared(size_t sh_count, size_t wk_count, All alloc,
                         Args&&... value)
      : BaseControlBlock(sh_count, wk_count),
        value_(std::forward<Args>(value)...),
        all_(alloc) {}

  void decreaseShared(void*) {
    if (--sharedCount_ != 0) {
      return;
    }
    std::allocator_traits<All>::destroy(all_, &(this->value_));
    delete_block();
  }
};

template <typename T>
class SharedPtr {
 private:
  T* ptr_ = nullptr;
  BaseControlBlock* block_ = nullptr;

  template <typename All>
  SharedPtr(ControlBlockMakeShared<T, All>* block)
      : ptr_(&(block->value_)), block_(block) {}

  SharedPtr(BaseControlBlock* block, T* ptr) : ptr_(ptr), block_(block) {
    block_->increase_shared_counter();
  }

 public:
  template <typename U>
  friend class SharedPtr;

  template <typename U>
  friend class WeakPtr;

  template <typename U, typename Alloc, typename... Args>
  friend SharedPtr<U> allocateShared(Alloc&, Args&&...);

  SharedPtr(T* ptr) : ptr_(ptr), block_(new ControlBlockRegular<T, T>(1, 0)) {}

  template <IsFamily<T> U>
  SharedPtr(U* ptr) : ptr_(ptr), block_(new ControlBlockRegular<T, U>(1, 0)) {}

  SharedPtr(const SharedPtr<T>& other)
      : ptr_(other.ptr_), block_(other.block_) {
    if (block_) {
      block_->increase_shared_counter();
    }
  }

  template <IsFamily<T> U>
  SharedPtr(const SharedPtr<U>& other)
      : ptr_(other.ptr_), block_(other.block_) {
    if (block_) {
      block_->increase_shared_counter();
    }
  }

  SharedPtr(const SharedPtr<T>& other, T* ptr)
      : ptr_(ptr), block_(other.block_) {
    if (block_) {
      block_->increase_shared_counter();
    }
  }

  template <IsFamily<T> U>
  SharedPtr(const SharedPtr<U>& other, T* ptr)
      : ptr_(ptr), block_(other.block_) {
    if (block_) {
      block_->increase_shared_counter();
    }
  }

  SharedPtr(SharedPtr<T>&& other) : ptr_(other.ptr_), block_(other.block_) {
    other.ptr_ = nullptr;
    other.block_ = nullptr;
  }

  template <IsFamily<T> U>
  SharedPtr(SharedPtr<U>&& other) : ptr_(other.ptr_), block_(other.block_) {
    other.ptr_ = nullptr;
    other.block_ = nullptr;
  }

  template <IsFamily<T> U, typename Del>
  SharedPtr(U* ptr, Del del)
      : ptr_(ptr), block_(new ControlBlockRegular<T, U, Del>(1, 0, del)) {}

  template <IsFamily<T> U, typename Del, typename All>
  SharedPtr(U* ptr, Del del, All alloc) : ptr_(ptr) {
    typename std::allocator_traits<All>::template rebind_alloc<
        ControlBlockRegular<T, U, Del, All>>
        tmp_alloc(alloc);

    auto* new_bc_ptr = std::allocator_traits<
        typename std::allocator_traits<All>::template rebind_alloc<
            ControlBlockRegular<T, U, Del, All>>>::allocate(tmp_alloc, 1);
    new (new_bc_ptr) ControlBlockRegular<T, U, Del, All>(1, 0, del, alloc);
    block_ = new_bc_ptr;
  }

  SharedPtr() = default;

  ~SharedPtr() {
    if (block_) {
      block_->decreaseShared(ptr_);
    }
  }

  SharedPtr& operator=(const SharedPtr<T>& other) {
    if (this == &other) {
      return *this;
    }
    if (block_) {
      block_->decreaseShared(ptr_);
    }

    ptr_ = other.ptr_;
    block_ = other.block_;

    if (block_) {
      block_->increase_shared_counter();
    }
    return *this;
  }

  template <IsFamily<T> U>
  SharedPtr& operator=(const SharedPtr<U>& other) {
    if (block_) {
      block_->decreaseShared(ptr_);
    }

    ptr_ = other.ptr_;
    block_ = other.block_;

    if (block_) {
      block_->increase_shared_counter();
    }
    return *this;
  }

  SharedPtr& operator=(SharedPtr<T>&& other) {
    if (block_ != nullptr) {
      block_->decreaseShared(ptr_);
    }
    ptr_ = nullptr;
    block_ = nullptr;

    auto* other_ptr = reinterpret_cast<T*>(other.ptr_);

    std::swap(ptr_, other_ptr);
    std::swap(block_, other.block_);
    return *this;
  }

  template <IsFamily<T> U>
  SharedPtr& operator=(SharedPtr<U>&& other) {
    if (block_ != nullptr) {
      block_->decreaseShared(ptr_);
    }
    ptr_ = nullptr;
    block_ = nullptr;

    auto* other_ptr = reinterpret_cast<T*>(other.ptr_);

    std::swap(ptr_, other_ptr);
    std::swap(block_, other.block_);
    return *this;
  }

  size_t use_count() const {
    if (block_) {
      return block_->sharedCount_;
    }
    return 0;
  }

  void reset(T* ptr) { SharedPtr(ptr).swap(*this); }

  template <IsFamily<T> U>
  void reset(U* ptr) {
    SharedPtr(ptr).swap(*this);
  }

  void reset() { SharedPtr().swap(*this); }

  T& operator*() const { return *ptr_; }

  T* get() const { return ptr_; }

  T* operator->() const { return ptr_; }


  template <IsFamily<T> U>
  void swap(SharedPtr<U>& other) {
    std::swap(block_, other.block_);
    std::swap(ptr_, other.ptr_);
  }
};

template <typename T>
class WeakPtr {
  T* ptr_ = nullptr;
  BaseControlBlock* block_ = nullptr;

  template <typename U>
  friend class SharedPtr;

  template <typename U>
  friend class WeakPtr;

  template <typename U, typename Alloc, typename... Args>
  friend SharedPtr<U> allocateShared(Alloc&, Args&&...);

 public:
  WeakPtr(const SharedPtr<T>& shared)
      : ptr_(shared.ptr_), block_(shared.block_) {
    if (block_) {
      block_->increase_weak_counter();
    }
  }

  template <IsFamily<T> U>
  WeakPtr(const SharedPtr<U>& shared)
      : ptr_(shared.ptr_), block_(shared.block_) {
    if (block_) {
      block_->increase_weak_counter();
    }
  }

  WeakPtr(const WeakPtr<T>& other) : ptr_(other.ptr_), block_(other.block_) {
    block_->increase_weak_counter();
  }

  template <IsFamily<T> U>
  WeakPtr(const WeakPtr<U>& other) : ptr_(other.ptr_), block_(other.block_) {
    block_->increase_weak_counter();
  }

  WeakPtr(WeakPtr<T>&& other) : ptr_(other.ptr_), block_(other.block_) {
    block_->increase_weak_counter();
  }

  template <IsFamily<T> U>
  WeakPtr(WeakPtr<U>&& other) : ptr_(other.ptr_), block_(other.block_) {
    block_->increase_weak_counter();
  }

  WeakPtr() = default;

  ~WeakPtr() {
    if (block_) {
      block_->decreaseWeak();
    }
  }

  WeakPtr& operator=(const WeakPtr<T>& other) {
    if (this == &other) {
      return *this;
    }
    if (block_) {
      block_->decreaseWeak();
    }
    ptr_ = other.ptr_;
    block_ = other.block_;
    return *this;
  }

  template <IsFamily<T> U>
  WeakPtr& operator=(const WeakPtr<U>& other) {
    if (this == &other) {
      return *this;
    }
    if (block_) {
      block_->decreaseWeak();
    }
    ptr_ = other.ptr_;
    block_ = other.block_;

    return *this;
  }

  WeakPtr& operator=(WeakPtr<T>&& other) & {
    if (this == &other) {
      return *this;
    }
    if (block_) {
      block_->decreaseWeak();
    }
    ptr_ = nullptr;
    block_ = nullptr;

    auto* other_ptr = reinterpret_cast<T*>(other.ptr_);

    std::swap(ptr_, other_ptr);
    std::swap(block_, other.block_);
    return *this;
  }

  template <IsFamily<T> U>
  WeakPtr& operator=(WeakPtr<U>&& other) {
    if (this == &other) {
      return *this;
    }
    if (block_) {
      block_->decreaseWeak();
    }
    ptr_ = nullptr;
    block_ = nullptr;

    auto* other_ptr = reinterpret_cast<T*>(other.ptr_);

    std::swap(ptr_, other_ptr);
    std::swap(block_, other.block_);
    return *this;
  }

  bool expired() const {
    if (!block_) {
      return true;
    }
    if (block_->sharedCount_ == 0) {
      return true;
    }
    return false;
  }

  SharedPtr<T> lock() const {
    if (expired()) {
      return SharedPtr<T>();
    }
    return SharedPtr<T>(block_, ptr_);
  }

  size_t use_count() const {
    if (block_) {
      return block_->sharedCount_;
    }
    return 0;
  }
};

template <typename T, typename All, typename... Args>
auto allocateShared(All& alloc, Args&&... args) -> SharedPtr<T> {
  typename std::allocator_traits<All>::template rebind_alloc<
      ControlBlockMakeShared<T, All>>
      block_all(alloc);

  auto* ptr = std::allocator_traits<
      typename std::allocator_traits<All>::template rebind_alloc<
          ControlBlockMakeShared<T, All>>>::allocate(block_all, 1);
  std::allocator_traits<All>::construct(alloc, ptr, 1, 0, alloc,
                                        std::forward<Args>(args)...);
  auto res = SharedPtr<T>(ptr);
  return res;
}

template <typename T, typename... Args>
SharedPtr<T> makeShared(Args&&... args) {
  std::allocator<T> time_all;
  auto res =
      allocateShared<T, std::allocator<T>, Args...>(time_all, std::forward<Args>(args)...);
  return res;
}