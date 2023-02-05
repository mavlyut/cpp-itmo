#include "dummy_vector.h"
#include <cstdlib>

dummy_vector::dummy_vector() = default;

dummy_vector::dummy_vector(dummy_vector const& other) {
  copy(other.data_, other.size_, other.size_);
}

dummy_vector& dummy_vector::operator=(const dummy_vector& other) {
  if (this != &other) {
    dummy_vector(other).swap(*this);
  }
  return *this;
}

dummy_vector::~dummy_vector() {
  clear();
  operator delete(data_);
}

dummy_vector::T& dummy_vector::operator[](size_t i) {
  return data_[i];
}

const dummy_vector::T &dummy_vector::operator[](size_t i) const {
  return data_[i];
}

dummy_vector::T* dummy_vector::data() {
  return data_;
}

const dummy_vector::T* dummy_vector::data() const {
  return data_;
}

size_t dummy_vector::size() const {
  return size_;
}

dummy_vector::T& dummy_vector::front() {
  return data_[0];
}

const dummy_vector::T& dummy_vector::front() const {
  return data_[0];
}

dummy_vector::T& dummy_vector::back() {
  return data_[size_ - 1];
}

const dummy_vector::T &dummy_vector::back() const {
  return data_[size_ - 1];
}

void dummy_vector::push_back(dummy_vector::T const& x) {
  if (size_ == capacity_) {
    size_t new_capacity = std::max<size_t>(1, 2 * capacity_);
    T* new_data = static_cast<T*>(operator new(new_capacity * sizeof(T)));
    for (size_t i = 0; i < size_; i++) {
      new (new_data + i) T(data_[i]);
      data_[i].~T();
    }
    new (new_data + size_) T(x);
    operator delete(data_);
    data_ = new_data;
    capacity_ = new_capacity;
  } else {
    new (data_ + size_) T(x);
  }
  size_++;
}

void dummy_vector::pop_back() {
  data_[--size_].~T();
}

bool dummy_vector::empty() const {
  return size_ == 0;
}

size_t dummy_vector::capacity() const {
  return capacity_;
}

void dummy_vector::reserve(size_t new_capacity) {
  if (new_capacity > capacity_) {
    set_capacity(new_capacity);
  }
}

void dummy_vector::shrink_to_fit() {
  set_capacity(size_);
}

void dummy_vector::clear() {
  erase(begin(), end());
}

void dummy_vector::swap(dummy_vector& other) {
  std::swap(data_, other.data_);
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
}

dummy_vector::iterator dummy_vector::begin() {
  return data_;
}

dummy_vector::iterator dummy_vector::end() {
  return data_ + size_;
}

dummy_vector::const_iterator dummy_vector::begin() const {
  return data_;
}

dummy_vector::const_iterator dummy_vector::end() const {
  return data_ + size_;
}

dummy_vector::iterator dummy_vector::insert(
    dummy_vector::const_iterator pos,
    const dummy_vector::T& ins
) {
  ptrdiff_t diff = pos - begin();
  push_back(ins);
  for (size_t i = size_ - 1; i - diff > 0; i--) {
    std::swap(data_[i], data_[i - 1]);
  }
  return begin() + diff;
}

dummy_vector::iterator dummy_vector::erase(dummy_vector::const_iterator pos) {
  return erase(pos, pos + 1);
}

dummy_vector::iterator dummy_vector::erase(
    dummy_vector::const_iterator first,
    dummy_vector::const_iterator last
) {
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

void dummy_vector::set_capacity(size_t new_capacity) {
  if (new_capacity != capacity_) {
    copy(data_, size_, new_capacity);
  }
}

void dummy_vector::copy(dummy_vector::T* data, size_t size, size_t capacity) {
  if (size <= capacity) {
    T* tmp_data = nullptr;
    if (capacity > 0) {
      tmp_data = static_cast<T*>(operator new(capacity * sizeof(T)));
    }
    for (size_t tmp_size = 0; tmp_size < size; tmp_size++) {
      new (tmp_data + tmp_size) T(data[tmp_size]);
    }
    clear();
    operator delete(data_);
    size_ = size;
    capacity_ = capacity;
    data_ = tmp_data;
  }
}

