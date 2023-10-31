#pragma once

#include <exception>
#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <vector>

template <typename T, typename Allocator = std::allocator<T>>
class Deque {
 private:
  static const size_t kArraySize = 1000;
  static const size_t kInitArrayAmount = 5;

  std::vector<T*> deque_;
  size_t size_ = 0;

  size_t first_arr_ = 0;
  size_t last_arr_ = 0;
  size_t first_pos_ = 0;
  size_t last_pos_ = 0;

  Allocator alloc_;
  using alloc_traits = std::allocator_traits<Allocator>;

 public:
  Deque() {
    deque_.resize(kInitArrayAmount);
    first_arr_ = kInitArrayAmount / 2;
    deque_[first_arr_] = alloc_traits::allocate(alloc_, kArraySize);
    last_arr_ = first_arr_;
  }

  Deque(const Allocator& alloc) {
    deque_.resize(kInitArrayAmount);
    first_arr_ = kInitArrayAmount / 2;
    deque_[first_arr_] = alloc_traits::allocate(alloc, kArraySize);
    last_arr_ = first_arr_;
  }

  Deque(const Deque& other) {
    size_ = other.size_;
    first_arr_ = other.first_arr_;
    first_pos_ = other.first_pos_;
    last_arr_ = other.last_arr_;
    last_pos_ = other.last_pos_;

    alloc_ = alloc_traits::select_on_container_copy_construction(other.alloc_);
    deque_.resize(other.deque_.size());
    size_t delete_to_arr = first_arr_;
    size_t delete_to_pos = 0;
    try {
      for (size_t i = first_arr_; i < last_arr_; ++i, ++delete_to_arr) {
        deque_[i] = alloc_traits::allocate(alloc_, kArraySize);
        for (size_t j = i == first_arr_ ? first_pos_ : 0; j < kArraySize;
             ++j, ++delete_to_pos) {
          alloc_traits::construct(alloc_, deque_[i] + j, other.deque_[i][j]);
        }
        delete_to_pos = 0;
      }

      deque_[last_arr_] = alloc_traits::allocate(alloc_, kArraySize);

      if (other.deque_[other.last_arr_] != nullptr) {
        for (size_t i = first_arr_ == last_arr_ ? first_pos_ : 0; i < last_pos_;
             ++i, ++delete_to_pos) {
          alloc_traits::construct(alloc_, deque_[last_arr_] + i,
                                  other.deque_[last_arr_][i]);
        }
        delete_to_pos = 0;
      }
    } catch (...) {
      for (size_t i = first_arr_; i < delete_to_arr; ++i) {
        for (size_t j = first_pos_; j < kArraySize; ++j) {
          alloc_traits::destroy(alloc_, deque_[i] + j);
        }
        alloc_traits::deallocate(alloc_, deque_[i], kArraySize);
        first_pos_ = 0;
      }
      for (size_t i = 0; i < delete_to_pos; ++i) {
        alloc_traits::destroy(alloc_, deque_[last_arr_] + i);
      }
      alloc_traits::deallocate(alloc_, deque_[last_arr_], kArraySize);
      throw;
    }
  }

  Deque(size_t count, const Allocator& alloc = Allocator()) {
    size_ = count;
    alloc_ = alloc;

    deque_.resize((count / kArraySize + 1) * 3);
    first_arr_ = deque_.size() / 3;
    last_arr_ = first_arr_ + count / kArraySize;

    first_pos_ = 0;
    last_pos_ = count % kArraySize;

    size_t delete_to_arr = first_arr_;
    size_t delete_to_pos = 0;

    try {
      for (size_t i = first_arr_; i < last_arr_; ++i, ++delete_to_arr) {
        deque_[i] = alloc_traits::allocate(alloc_, kArraySize);
        for (size_t j = 0; j < kArraySize; ++j, ++delete_to_pos) {
          alloc_traits::construct(alloc_, deque_[i] + j);
        }
        delete_to_pos = 0;
      }
      deque_[last_arr_] = alloc_traits::allocate(alloc_, kArraySize);
      for (size_t i = 0; i < last_pos_; ++i, ++delete_to_pos) {
        alloc_traits::construct(alloc_, deque_[last_arr_] + i);
      }
    } catch (...) {
      for (size_t i = first_arr_; i < delete_to_arr; ++i) {
        for (size_t j = 0; j < kArraySize; ++j) {
          alloc_traits::destroy(alloc_, deque_[i] + j);
        }
        alloc_traits::deallocate(alloc_, deque_[i], kArraySize);
      }
      for (size_t i = 0; i < delete_to_pos; ++i) {
        alloc_traits::destroy(alloc_, deque_[delete_to_arr] + i);
      }
      alloc_traits::deallocate(alloc_, deque_[delete_to_arr], kArraySize);
      throw;
    }
  }

  Deque(size_t count, const T& value, const Allocator& alloc = Allocator()) {
    size_ = count;
    alloc_ = alloc;

    deque_.resize((count / kArraySize + 1) * 3);
    first_arr_ = deque_.size() / 3;
    last_arr_ = first_arr_ + count / kArraySize;

    first_pos_ = 0;
    last_pos_ = count % kArraySize;

    size_t delete_to_arr = first_arr_;
    size_t delete_to_pos = 0;

    try {
      for (size_t i = first_arr_; i < last_arr_; ++i, ++delete_to_arr) {
        deque_[i] = alloc_traits::allocate(alloc_, kArraySize);
        for (size_t j = 0; j < kArraySize; ++j, ++delete_to_pos) {
          alloc_traits::construct(alloc_, deque_[i] + j, value);
        }
        delete_to_pos = 0;
      }
      deque_[last_arr_] = alloc_traits::allocate(alloc_, kArraySize);
      for (size_t i = 0; i < last_pos_; ++i, ++delete_to_pos) {
        alloc_traits::construct(alloc_, deque_[last_arr_] + i, value);
      }
    } catch (...) {
      for (size_t i = first_arr_; i < delete_to_arr; ++i) {
        for (size_t j = 0; j < kArraySize; ++j) {
          alloc_traits::destroy(alloc_, deque_[i] + j);
        }
        alloc_traits::deallocate(alloc_, deque_[i], kArraySize);
      }
      for (size_t i = 0; i < delete_to_pos; ++i) {
        alloc_traits::destroy(alloc_, deque_[delete_to_arr] + i);
      }
      alloc_traits::deallocate(alloc_, deque_[delete_to_arr], kArraySize);
      throw;
    }
  }

  Deque(Deque&& other) {
    size_ = other.size_;
    first_arr_ = other.first_arr_;
    last_arr_ = other.last_arr_;
    first_pos_ = other.first_pos_;
    last_pos_ = other.last_pos_;
    deque_ = std::move(other.deque_);
    alloc_ = std::move(other.alloc_);

    other.deque_.clear();
    other.size_ = 0;
    other.first_arr_ = deque_.size() / 2;
    other.first_pos_ = 0;
    other.last_arr_ = other.first_arr_;
    other.last_pos_ = other.first_pos_;
  }

  Deque(std::initializer_list<T> init, const Allocator& alloc = Allocator()) {
    size_ = init.size();
    alloc_ = alloc;

    deque_.resize((size_ / kArraySize + 1) * 3);
    first_arr_ = deque_.size() / 3;
    last_arr_ = first_arr_ + size_ / kArraySize;
    first_pos_ = 0;
    last_pos_ = size_ % kArraySize;

    size_t delete_to_arr = first_arr_;
    size_t delete_to_pos = 0;

    try {
      auto init_begin = init.begin();
      for (size_t i = first_arr_; i < last_arr_; ++i, ++delete_to_arr) {
        deque_[i] = alloc_traits::allocate(alloc_, kArraySize);
        for (size_t j = 0; j < kArraySize; ++j, ++delete_to_pos) {
          alloc_traits::construct(alloc_, deque_[i] + j, *init_begin++);
        }
        delete_to_pos = 0;
      }
      deque_[last_arr_] = alloc_traits::allocate(alloc_, kArraySize);

      for (size_t i = 0; i < last_pos_; ++i, ++delete_to_pos) {
        alloc_traits::construct(alloc_, deque_[last_arr_] + i, *init_begin++);
      }
    } catch (...) {
      for (size_t i = first_arr_; i < delete_to_arr; ++i) {
        for (size_t j = 0; j < kArraySize; ++j) {
          alloc_traits::destroy(alloc_, deque_[i] + j);
        }
        alloc_traits::deallocate(alloc_, deque_[i], kArraySize);
      }
      for (size_t i = 0; i < delete_to_pos; ++i) {
        alloc_traits::destroy(alloc_, deque_[delete_to_arr] + i);
      }
      alloc_traits::deallocate(alloc_, deque_[delete_to_arr], kArraySize);
      throw;
    }
  }

  Deque& operator=(const Deque& other) {
    if (alloc_traits::propagate_on_container_copy_assignment::value &&
        alloc_ != other.alloc_) {
      alloc_ = other.alloc_;
    }
    Deque other_copy(other);
    std::swap(deque_, other_copy.deque_);
    std::swap(size_, other_copy.size_);
    std::swap(first_arr_, other_copy.first_arr_);
    std::swap(last_arr_, other_copy.last_arr_);
    std::swap(first_pos_, other_copy.first_pos_);
    std::swap(last_pos_, other_copy.last_pos_);

    return *this;
  }

  Deque& operator=(Deque&& other) {
    if (*this != other) {
      alloc_ = std::move(other.alloc_);
      std::swap(deque_, other.deque_);
      std::swap(size_, other.size_);
      std::swap(first_arr_, other.first_arr_);
      std::swap(last_arr_, other.last_arr_);
      std::swap(first_pos_, other.first_pos_);
      std::swap(last_pos_, other.last_pos_);

      for (size_t i = other.first_arr_; i < other.last_arr_; ++i) {
        for (size_t j = other.first_pos_; j < kArraySize; ++j) {
          alloc_traits::destroy(alloc_, other.deque_[i] + j);
        }
        alloc_traits::deallocate(alloc_, other.deque_[i], kArraySize);
        other.first_pos_ = 0;
      }
      for (size_t i = 0; i < other.last_pos_; ++i) {
        alloc_traits::destroy(alloc_, other.deque_[other.last_arr_] + i);
      }
      alloc_traits::deallocate(alloc_, other.deque_[other.last_arr_],
                               kArraySize);
    }
    return *this;
  }

  ~Deque() {
    if (deque_.empty() || deque_[first_arr_] == nullptr ||
        deque_[last_arr_] == nullptr) {
      return;
    }
    for (size_t i = first_arr_; i < last_arr_; ++i) {
      for (size_t j = first_pos_; j < kArraySize; ++j) {
        alloc_traits::destroy(alloc_, deque_[i] + j);
      }
      alloc_traits::deallocate(alloc_, deque_[i], kArraySize);
      first_pos_ = 0;
    }
    for (size_t i = first_pos_; i < last_pos_; ++i) {
      alloc_traits::destroy(alloc_, deque_[last_arr_] + i);
    }
    alloc_traits::deallocate(alloc_, deque_[last_arr_], kArraySize);
  }

  T& operator[](size_t num) {
    if ((first_pos_ + num) < kArraySize) {
      return deque_[first_arr_][first_pos_ + num];
    }
    num -= kArraySize - first_pos_;
    size_t needed_arr = first_arr_ + num / kArraySize + 1;
    size_t needed_pos = num % kArraySize;
    return deque_[needed_arr][needed_pos];
  }

  const T& operator[](size_t num) const {
    if ((first_pos_ + num) < kArraySize) {
      return deque_[first_arr_][first_pos_ + num];
    }
    num -= kArraySize - first_pos_;
    size_t needed_arr = first_arr_ + num / kArraySize + 1;
    size_t needed_pos = num % kArraySize;
    return deque_[needed_arr][needed_pos];
  }

  T& at(size_t num) {
    if (num >= size_) {
      throw std::out_of_range("Out of range");
    }
    if ((first_pos_ + num) < kArraySize) {
      return deque_[first_arr_][first_pos_ + num];
    }
    num -= kArraySize - first_pos_;
    size_t needed_arr = first_arr_ + num / kArraySize + 1;
    size_t needed_pos = num % kArraySize;
    return deque_[needed_arr][needed_pos];
  }

  const T& at(size_t num) const {
    if (num >= size_) {
      throw std::out_of_range("Out of range");
    }
    if ((first_pos_ + num) < kArraySize) {
      return deque_[first_arr_][first_pos_ + num];
    }
    num -= kArraySize - first_pos_;
    size_t needed_arr = first_arr_ + num / kArraySize + 1;
    size_t needed_pos = num % kArraySize;
    return deque_[needed_arr][needed_pos];
  }

  size_t size() const { return size_; }

  bool empty() { return size_ == 0; }

  Allocator get_allocator() { return alloc_; }

  void resize_deque() {
    size_t size = deque_.size();
    std::vector<T*> new_deque(size * 3);
    first_arr_ += size;
    last_arr_ += size;
    std::copy(deque_.begin(), deque_.end(), new_deque.begin() + size);
    deque_.clear();
    deque_ = new_deque;
  }

  void push_back(const T& value) {
    if (last_arr_ == deque_.size() - 1) {
      if (last_pos_ == kArraySize - 1) {
        resize_deque();
      }
    }

    try {
      alloc_traits::construct(alloc_, deque_[last_arr_] + last_pos_, value);
      ++size_;
      ++last_pos_;

      if (last_pos_ == kArraySize) {
        ++last_arr_;
        last_pos_ = 0;
        deque_[last_arr_] = alloc_traits::allocate(alloc_, kArraySize);
      }
    } catch (...) {
      alloc_traits::destroy(alloc_, deque_[last_arr_] + last_pos_);
      throw;
    }
  }

  void push_back(T&& value) {
    if (last_arr_ == deque_.size() - 1) {
      if (last_pos_ == kArraySize - 1) {
        resize_deque();
      }
    }

    try {
      alloc_traits::construct(alloc_, deque_[last_arr_] + last_pos_,
                              std::move(value));
    } catch (...) {
      alloc_traits::destroy(alloc_, deque_[last_arr_] + last_pos_);
      throw;
    }

    ++size_;
    ++last_pos_;

    if (last_pos_ == kArraySize) {
      ++last_arr_;
      last_pos_ = 0;
      deque_[last_arr_] = alloc_traits::allocate(alloc_, kArraySize);
    }
  }

  void push_front(const T& value) {
    size_t copy_first_arr = first_arr_;
    size_t copy_first_pos = first_pos_;

    if (first_arr_ == 0) {
      if (first_pos_ == 0) {
        resize_deque();
      }
    }

    if (first_pos_ == 0) {
      first_pos_ = kArraySize;
      --first_arr_;
      deque_[first_arr_] = alloc_traits::allocate(alloc_, kArraySize);
    }

    try {
      alloc_traits::construct(alloc_, deque_[first_arr_] + first_pos_ - 1,
                              std::move(value));
    } catch (...) {
      alloc_traits::destroy(alloc_, deque_[first_arr_] + first_pos_ - 1);
      first_arr_ = copy_first_arr;
      first_pos_ = copy_first_pos;
      throw;
    }

    --first_pos_;
    ++size_;
  }

  void push_front(T&& value) {
    size_t copy_first_arr = first_arr_;
    size_t copy_first_pos = first_pos_;

    if (first_arr_ == 0) {
      if (first_pos_ == 0) {
        resize_deque();
      }
    }

    if (first_pos_ == 0) {
      first_pos_ = kArraySize;
      --first_arr_;
      deque_[first_arr_] = alloc_traits::allocate(alloc_, kArraySize);
    }

    try {
      alloc_traits::construct(alloc_, deque_[first_arr_] + first_pos_ - 1,
                              std::move(value));
    } catch (...) {
      alloc_traits::destroy(alloc_, deque_[first_arr_] + first_pos_ - 1);
      first_arr_ = copy_first_arr;
      first_pos_ = copy_first_pos;
      throw;
    }

    --first_pos_;
    ++size_;
  }

  void pop_back() {
    --size_;
    if (last_pos_ == 0) {
      alloc_traits::destroy(alloc_, deque_[last_arr_] + last_pos_);
      alloc_traits::deallocate(alloc_, deque_[last_arr_], kArraySize);
      --last_arr_;
      last_pos_ = kArraySize - 1;
    } else {
      alloc_traits::destroy(alloc_, deque_[last_arr_] + last_pos_);
      --last_pos_;
    }
  }

  void pop_front() {
    --size_;
    if (first_pos_ == kArraySize - 1) {
      alloc_traits::destroy(alloc_, deque_[first_arr_] + first_pos_ - 1);
      alloc_traits::deallocate(alloc_, deque_[first_arr_], kArraySize);
      ++first_arr_;
      first_pos_ = 0;
    } else {
      alloc_traits::destroy(alloc_, deque_[first_arr_] + first_pos_);
      ++first_pos_;
    }
  }

  template <class... Args>
  void emplace_front(Args&&... elems) {
    if (first_arr_ == 0) {
      if (first_arr_ == 0) {
        resize_deque();
      }
    }

    try {
      alloc_traits::construct(alloc_, deque_[first_arr_] + first_pos_ - 1,
                              std::forward<Args>(elems)...);
    } catch (...) {
      alloc_traits::destroy(alloc_, deque_[first_arr_] + first_pos_ - 1);
      throw;
    }

    ++size_;

    if (first_pos_ == 0) {
      --first_arr_;
      first_pos_ = kArraySize - 1;
      deque_[first_arr_] = alloc_traits::allocate(alloc_, kArraySize);
    } else {
      --first_pos_;
    }
  }

  template <class... Args>
  void emplace_back(Args&&... elems) {
    if (last_arr_ == deque_.size() - 1) {
      if (last_pos_ == kArraySize - 1) {
        resize_deque();
      }
    }

    try {
      alloc_traits::construct(alloc_, deque_[last_arr_] + last_pos_,
                              std::forward<Args>(elems)...);
    } catch (...) {
      alloc_traits::destroy(alloc_, deque_[last_arr_] + last_pos_);
      throw;
    }

    ++size_;
    ++last_pos_;

    if (last_pos_ == kArraySize) {
      ++last_arr_;
      last_pos_ = 0;
      deque_[last_arr_] = alloc_traits::allocate(alloc_, kArraySize);
    }
  }

  template <bool IsConst>
  class Iterator {
   private:
    T** ptr_;
    size_t pos_;

   public:
    using value_type = T;
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = int;
    using is_const = std::conditional_t<IsConst, const T, T>;
    using pointer = is_const*;
    using reference = is_const&;

    Iterator(T** ptr, const size_t& pos) : ptr_(ptr), pos_(pos) {}

    Iterator operator++(int) {
      Iterator copy(*this);
      ++*this;
      return copy;
    }

    Iterator& operator++() {
      if (pos_ == kArraySize - 1) {
        ++ptr_;
        pos_ = 0;
      } else {
        ++pos_;
      }
      return *this;
    }

    Iterator operator--(int) {
      Iterator copy(*this);
      --*this;
      return copy;
    }

    Iterator& operator--() {
      if (pos_ == 0) {
        --ptr_;
        pos_ = kArraySize - 1;
      } else {
        --pos_;
      }
      return *this;
    }

    Iterator& operator+=(int value) {
      if (value < 0) {
        *this -= (-value);
        return *this;
      }

      pos_ += value;
      ptr_ += pos_ / kArraySize;
      pos_ = pos_ % kArraySize;

      return *this;
    }

    Iterator& operator-=(int value) {
      if (value < 0) {
        *this += (-value);
        return *this;
      }

      if (static_cast<int>(pos_) - value < 0) {
        value -= pos_ + 1;
        ptr_ -= value / kArraySize + 1;
        pos_ = kArraySize - 1 - (value % kArraySize);
        return *this;
      }

      pos_ -= value;
      return *this;
    }

    friend Iterator operator+(const Iterator& iter, int value) {
      Iterator copy(iter);
      copy += value;
      return copy;
    }

    friend Iterator operator-(const Iterator& iter, int value) {
      Iterator copy(iter);
      copy -= value;
      return copy;
    }

    difference_type operator-(const Iterator& other) {
      return (ptr_ - other.ptr_) * kArraySize + (pos_ - other.pos_);
    }

    friend bool operator<(const Iterator& first, const Iterator& other) {
      return (first.ptr_ < other.ptr_) ||
             (first.ptr_ == other.ptr_ && first.pos_ < other.pos_);
    }

    friend bool operator<=(const Iterator& first, const Iterator& other) {
      return ((first.ptr_ < other.ptr_) ||
              (first.ptr_ == other.ptr_ && first.pos_ < other.pos_)) ||
             ((first.ptr_ == other.ptr_) && (first.pos_ == other.pos_));
    }

    friend bool operator>(const Iterator& first, const Iterator& other) {
      return (first.ptr_ > other.ptr_) ||
             (first.ptr_ == other.ptr_ && first.pos_ > other.pos_);
    }

    friend bool operator>=(const Iterator& first, const Iterator& other) {
      return ((first.ptr_ > other.ptr_) ||
              (first.ptr_ == other.ptr_ && first.pos_ > other.pos_)) ||
             ((first.ptr_ == other.ptr_) && (first.pos_ == other.pos_));
    }

    friend bool operator==(const Iterator& first, const Iterator& other) {
      return (first.ptr_ == other.ptr_) && (first.pos_ == other.pos_);
    }

    friend bool operator!=(const Iterator& first, const Iterator& other) {
      return !((first.ptr_ == other.ptr_) && (first.pos_ == other.pos_));
    }

    reference operator*() const { return (*ptr_)[pos_]; }

    pointer operator->() const { return *ptr_ + pos_; }
  };

  using iterator = Iterator<false>;
  using reverse_iterator = std::reverse_iterator<Iterator<false>>;
  using const_iterator = Iterator<true>;
  using const_reverse_iterator = std::reverse_iterator<Iterator<true>>;

  iterator begin() { return iterator(&deque_[first_arr_], first_pos_); }

  iterator end() { return iterator(&deque_[last_arr_], last_pos_); }

  reverse_iterator rbegin() { return reverse_iterator(end()); }

  reverse_iterator rend() { return reverse_iterator(begin()); }

  const_iterator cbegin() {
    return const_iterator(&deque_[first_arr_], first_pos_);
  }

  const_iterator cend() {
    return const_iterator(&deque_[last_arr_], last_pos_);
  }

  const_reverse_iterator crbegin() { return const_reverse_iterator(end()); }

  const_reverse_iterator crend() { return const_reverse_iterator(begin()); }

  void insert(iterator iter, const T& elem) {
    if (iter == end()) {
      push_back(elem);
      return;
    }
    T tmp = elem;
    while (iter < end()) {
      auto other_tmp = *iter;
      *iter = tmp;
      tmp = other_tmp;
      iter++;
    }
    push_back(tmp);
  }

  void emplace(iterator iter, T&& elem) {
    if (iter == end()) {
      push_back(std::move(elem));
      return;
    }
    T tmp(std::move(elem));
    while (iter < end()) {
      auto other_tmp = *iter;
      *iter = tmp;
      tmp = other_tmp;
      iter++;
    }
    push_back(tmp);
  }

  void erase(iterator iter) {
    while (iter < end()) {
      *iter = *(iter + 1);
      ++iter;
    }
    --size_;
    if (last_pos_ == 0) {
      last_pos_ = kArraySize - 1;
      --last_arr_;
    } else {
      --last_pos_;
    }
  }
};
