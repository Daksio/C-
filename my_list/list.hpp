#pragma once

#include <iostream>

template <typename T, typename Allocator = std::allocator<T>>
class List {
 private:
  struct Node {
    T value;
    Node* next = nullptr;
    Node* prev = nullptr;

    Node() = default;
    Node(const T& n) : value(n) {}
    ~Node() = default;
  };

 public:
  using allocator_type = Allocator;
  using alloc_type = typename std::allocator_traits<
      allocator_type>::template rebind_alloc<Node>;
  using alloc_traits = std::allocator_traits<alloc_type>;

 private:
  size_t size_ = 0;
  Node* head_ = nullptr;
  Node* tail_ = nullptr;

  alloc_type allocator_ = Allocator();

 public:
  using value_type = T;
  using reference = value_type&;
  using pointer = value_type*;
  List() = default;

  List(size_t count, const T& value, const Allocator& alloc = Allocator())
      : allocator_(alloc), head_(nullptr), tail_(nullptr), size_(0) {
    for (size_t i = 0; i < count; ++i) {
      push_back(value);
    }
  }

  explicit List(size_t count, const Allocator& alloc = Allocator())
      : allocator_(alloc), head_(nullptr), tail_(nullptr), size_(0) {
    for (size_t i = 0; i < count; ++i) {
      push_back();
    }
  }

  List(const List& other)
      : allocator_(
            std::allocator_traits<Allocator>::
                select_on_container_copy_construction(other.allocator_)) {
    Node* copy = other.head_;
    for (size_t i = 0; i < other.size_; ++i) {
      push_back(copy->value);
      copy = copy->next;
    }
  }

  List(std::initializer_list<T> init, const Allocator& alloc = Allocator())
      : allocator_(alloc), head_(nullptr), tail_(nullptr), size_(0) {
    for (auto it = init.begin(); it != init.end(); ++it) {
      push_back(*it);
    }
  }

  ~List() { clear_list(); }

  void clear_list() {
    while (!empty()) {
      pop_back();
    }
  }

  List& operator=(const List& other) {
    clear_list();
    if (std::allocator_traits<
            Allocator>::propagate_on_container_copy_assignment::value) {
      allocator_ = other.allocator_;
    }
    Node* copy = other.head_;
    for (size_t i = size_; i < other.size_; ++i) {
      try {
        push_back(copy->value);
      } catch (...) {
        throw;
      }
      copy = copy->next;
    }
    return *this;
  }

  T& front() { return head_->value; }
  const T& front() const { return head_->value; }

  T& back() { return tail_->value; }
  const T& back() const { return tail_->value; }

  bool empty() const { return size_ == 0; }
  size_t size() const { return size_; }

  alloc_type get_allocator() { return allocator_; }

  void push_back(const T& val) {
    Node* tmp;
    try {
      tmp = alloc_traits::allocate(allocator_, 1);
      alloc_traits::construct(allocator_, tmp, std::move(val));
      if (size() == 0) {
        tail_ = tmp;
        head_ = tmp;
        ++size_;
      } else {
        tail_->next = tmp;
        tmp->prev = tail_;
        tail_ = tail_->next;
        ++size_;
      }
    } catch (...) {
      alloc_traits::destroy(allocator_, tmp);
      alloc_traits::deallocate(allocator_, tmp, 1);
    }
  }

  void push_back(T&& val) {
    Node* tmp = alloc_traits::allocate(allocator_, 1);
    alloc_traits::construct(allocator_, tmp, val);
    if (size() == 0) {
      tail_ = tmp;
      head_ = tmp;
      ++size_;
    } else {
      tail_->next = tmp;
      tmp->prev = tail_;
      tail_ = tail_->next;
      ++size_;
    }
  }

  void push_back() {
    Node* tmp = alloc_traits::allocate(allocator_, 1);
    try {
      alloc_traits::construct(allocator_, tmp);
    } catch (...) {
      alloc_traits::deallocate(allocator_, tmp, 1);
      clear_list();
      throw;
    }
    if (size() == 0) {
      tail_ = tmp;
      head_ = tmp;
    } else {
      tail_->next = tmp;
      tmp->prev = tail_;
      tail_ = tail_->next;
    }
    ++size_;
  }

  void pop_back() {
    Node* node = tail_;
    if (size() != 1) {
      tail_ = tail_->prev;
      tail_->next = nullptr;
    } else {
      head_ = nullptr;
      tail_ = nullptr;
    }
    alloc_traits::destroy(allocator_, node);
    alloc_traits::deallocate(allocator_, node, 1);
    --size_;
  }

  void push_front(const T& val) {
    Node* new_node = alloc_traits::allocate(allocator_, 1);
    try {
      alloc_traits::construct(allocator_, new_node, val);
    } catch (...) {
      alloc_traits::deallocate(allocator_, new_node, 1);
      clear_list();
      throw;
    }
    if (size() == 0) {
      tail_ = new_node;
      head_ = new_node;
    } else {
      head_->prev = new_node;
      new_node->next = head_;
      head_ = head_->prev;
    }
    ++size_;
  }

  void push_front(T&& val) {
    Node* new_node = alloc_traits::allocate(allocator_, 1);
    try {
      alloc_traits::construct(allocator_, new_node, val);
    } catch (...) {
      alloc_traits::deallocate(allocator_, new_node, 1);
      clear_list();
      throw;
    }
    if (size() == 0) {
      tail_ = new_node;
      head_ = new_node;
    } else {
      head_->prev = new_node;
      new_node->next = head_;
      head_ = head_->prev;
    }
    ++size_;
  }

  void pop_front() {
    Node* node = head_;
    if (size() != 1) {
      head_ = head_->next;
      head_->prev = nullptr;
    } else {
      head_ = nullptr;
      tail_ = nullptr;
    }
    alloc_traits::destroy(allocator_, node);
    alloc_traits::deallocate(allocator_, node, 1);
    --size_;
  }

  template <bool IsConst>
  class Iterator {
   private:
    Node* cur_;
    Node* top_;
    Node* end_;

    size_t ind_;

   public:
    using value_type = T;
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = typename std::iterator<
        std::bidirectional_iterator_tag,
        typename std::conditional<IsConst, const T, T>::type>::difference_type;
    using pointer = typename std::iterator<
        std::bidirectional_iterator_tag,
        typename std::conditional<IsConst, const T, T>::type>::pointer;
    using reference = typename std::iterator<
        std::bidirectional_iterator_tag,
        typename std::conditional<IsConst, const T, T>::type>::reference;

    explicit Iterator<IsConst>(Node* cur1, Node* top1 = nullptr,
                               Node* end1 = nullptr, size_t ind1 = 0)
        : cur_(cur1), top_(top1), end_(end1), ind_(ind1){};

    Iterator& operator++() {
      ++ind_;
      if (cur_ == nullptr) {
        cur_ = end_;
      } else {
        cur_ = cur_->next;
      }
      return *this;
    }

    Iterator operator++(int) {
      Iterator it_copy(cur_);
      ++ind_;
      if (cur_ == nullptr) {
        cur_ = end_;
      } else {
        cur_ = cur_->next;
      }
      return it_copy;
    }

    Iterator& operator--() {
      --ind_;
      if (cur_ == nullptr) {
        cur_ = end_;
      } else {
        cur_ = cur_->prev;
      }
      return *this;
    }

    Iterator operator--(int) {
      Iterator it_copy(cur_);
      ++ind_;
      if (cur_ == nullptr) {
        cur_ = end_;
      } else {
        cur_ = cur_->prev;
      }
      return it_copy;
    }

    reference operator*() const { return cur_->value; }
    pointer operator->() const { return &(cur_->value); }

    bool operator==(const Iterator& second) const {
      if (ind_ == second.ind_) {
        if (cur_ == second.cur_) {
          return true;
        }
        if ((cur_->next == nullptr) ^ (second.cur_->next == nullptr) ||
            (cur_->prev == nullptr) ^ (second.cur_->prev == nullptr)) {
          return false;
        }
        return cur_->next == second.cur_->next &&
               cur_->prev == second.cur_->prev;
      }
      return false;
    }

    bool operator!=(const Iterator& second) const { return !(*this == second); }
  };

  using iterator = Iterator<false>;
  using const_iterator = Iterator<true>;

  iterator begin() { return iterator(head_, head_, tail_); }

  iterator rbegin() { return iterator(tail_, head_, tail_, size() - 1); }

  iterator end() { return iterator(tail_->next, head_, tail_, size()); }

  iterator rend() { return iterator(head_->prev, head_, tail_, -1); }

  const_iterator begin() const { return const_iterator(head_, head_, tail_); }

  const_iterator end() const {
    return const_iterator(tail_->next, head_, tail_, size());
  }

  const_iterator cbegin() { return const_iterator(head_, head_, tail_); }

  const_iterator cend() {
    return const_iterator(tail_->next, head_, tail_, size());
  }

  const_iterator crbegin() { const_iterator(tail_, head_, tail_, size() - 1); }

  const_iterator crend() {
    return const_iterator(head_->prev, head_, tail_, -1);
  }
};