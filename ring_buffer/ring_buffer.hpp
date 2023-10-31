#pragma once

#include <cstddef>
#include <vector>

class RingBuffer {
 private:
  std::vector<int> queue_;
  size_t last_ = 0;
  size_t size_ = 0;
  size_t max_size_ = 0;

 public:
  explicit RingBuffer(int capacity) {
    max_size_ = capacity;
    queue_.assign(max_size_, 0);
  }

  size_t Size() const { return size_; }

  bool Empty() const { return size_ == 0; }

  bool TryPush(int element) {
    if (size_ == max_size_) {
      return false;
    }
    queue_[last_] = element;
    size_++;
    last_++;
    return true;
  }

  bool TryPop(int* element) {
    if (size_ == 0) {
      return false;
    }
    *element = queue_[0];
    for (size_t i = 0; i < size_ - 1; i++) {
      queue_[i] = queue_[i + 1];
    }
    queue_[size_ - 1] = 0;
    size_--;
    last_--;
    return true;
  }
};