#pragma once
#include <cstddef>
#include <iostream>

template <typename T>
struct vector {
  using iterator = T*;
  using const_iterator = T const*;

  vector() = default;                                   // O(1) nothrow

  vector(vector const& other) {                         // O(N) strong
    update(other.data_, other.size_, other.size_);
  }

  vector& operator=(vector const& other) {              // O(N) strong
    if (this != &other) {
      vector(other).swap(*this);
    }
    return *this;
  }

  ~vector() {                                           // O(N) nothrow
    clear();
    operator delete(data_);
  }

  T& operator[](size_t i) {                             // O(1) nothrow
    return data_[i];
  }

  T const& operator[](size_t i) const {                 // O(1) nothrow
    return data_[i];
  }

  T* data() {                                           // O(1) nothrow
    return data_;
  }

  T const* data() const {                               // O(1) nothrow
    return data_;
  }

  size_t size() const {                                 // O(1) nothrow
    return size_;
  }

  T& front() {                                          // O(1) nothrow
    return data_[0];
  }

  T const& front() const {                              // O(1) nothrow
    return data_[0];
  }

  T& back() {                                           // O(1) nothrow
    return data_[size_ - 1];
  }

  T const& back() const {                               // O(1) nothrow
    return data_[size_ - 1];
  }

  void push_back(T const& x) {                          // O(1)* strong
    if (size_ == capacity_) {
      size_t new_capacity = std::max<size_t>(1, 2 * capacity_);
      T* new_data = copy(data_, size_, new_capacity);
      try {
        new (new_data + size_) T(x);
      } catch (...) {
        delete_array(new_data, size_);
        throw;
      }
      reset(new_data, size_ + 1, new_capacity);
    } else {
      new (data_ + size_) T(x);
      size_++;
    }
  }

  void pop_back() {                                     // O(1) nothrow
    data_[--size_].~T();
  }

  bool empty() const {                                  // O(1) nothrow
    return size_ == 0;
  }

  size_t capacity() const {                             // O(1) nothrow
    return capacity_;
  }

  void reserve(size_t new_capacity) {                   // O(N) strong
    if (new_capacity > capacity_) {
      set_capacity(new_capacity);
    }
  }

  void shrink_to_fit() {                                // O(N) strong
    set_capacity(size_);
  }

  void clear() {                                        // O(N) nothrow
    erase(begin(), end());
  }

  void swap(vector& other) {                            // O(1) nothrow
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
  }

  iterator begin() {                                    // O(1) nothrow
    return data_;
  }

  iterator end() {                                      // O(1) nothrow
    return data_ + size_;
  }

  const_iterator begin() const {                        // O(1) nothrow
    return data_;
  }

  const_iterator end() const {                          // O(1) nothrow
    return data_ + size_;
  }

  iterator insert(const_iterator pos, T const& ins) {   // O(N) strong
    ptrdiff_t diff = pos - begin();
    push_back(ins);
    for (size_t i = size_ - 1; i - diff > 0; i--) {
      std::swap(data_[i], data_[i - 1]);
    }
    return begin() + diff;
  }

  iterator erase(const_iterator pos) {                  // O(N) nothrow(swap)
    return erase(pos, pos + 1);
  }

  iterator erase(const_iterator first, const_iterator last) {       // O(N) nothrow(swap)
    ptrdiff_t count = last - first;
    ptrdiff_t start = first - begin();
    for (size_t i = start; i < size_ - count; i++) {
      std::swap(data_[i], data_[i + count]);
    }
    for (size_t i = 0; i < count; ++i) {
      pop_back();
    }
    return begin() + start;
  }

private:
  T* data_{nullptr};
  size_t size_{0};
  size_t capacity_{0};

  T* copy(T* data, size_t size, size_t capacity) {                  // O(N) strong
    T* tmp_data = nullptr;
    if (capacity > 0 && size <= capacity) {
      tmp_data = static_cast<T*>(operator new(capacity * sizeof(T)));
      size_t tmp_size = 0;
      try {
        for (; tmp_size < size; tmp_size++) {
          new (tmp_data + tmp_size) T(data[tmp_size]);
        }
      } catch (...) {
        delete_array(tmp_data, tmp_size);
        throw;
      }
    }
    return tmp_data;
  }

  void delete_array(T* data, size_t tmp_size) {                     // O(N) nothrow
    for (size_t i = 0; i < tmp_size; i++) {
      (data + i)->~T();
    }
    operator delete(data);
  }

  void reset(T* tmp_data, size_t tmp_size, size_t new_capacity) {   // O(N) nothrow
    delete_array(data_, size_);
    size_ = tmp_size;
    capacity_ = new_capacity;
    data_ = tmp_data;
  }

  void update(T* data, size_t size, size_t capacity) {              // O(N) strong
    reset(copy(data, size, capacity), size, capacity);
  }

  void set_capacity(size_t new_capacity) {                          // O(N) strong
    if (new_capacity != capacity_) {
      update(data_, size_, new_capacity);
    }
  }
};

