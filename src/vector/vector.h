#pragma once

#include <initializer_list>
#include <limits>

#include "vector_iterator.h"

namespace containers {

template <typename T>
class Vector {
 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;
  using iterator = VectorIterator<T>;
  using const_iterator = ConstVectorIterator<T>;

  Vector() = default;
  explicit Vector(size_type capacity, const_reference value = {});
  Vector(std::initializer_list<T> const& items);
  Vector(const Vector<T>& v);
  Vector(Vector<T>&& v) noexcept;
  ~Vector() = default;

  Vector& operator=(Vector&& v) noexcept;
  reference operator[](size_type pos);
  const_reference operator[](size_type pos) const;

  const_reference front() const;
  const_reference back() const;

  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;
  const_iterator cbegin() const;
  const_iterator cend() const;

  bool empty() const noexcept;
  size_type size() const noexcept;
  size_type max_size() const noexcept;
  void reserve(size_type new_cap);
  size_type capacity() const noexcept;
  void shrink_to_fit();

  reference at(const size_type pos);
  const_reference at(const size_type pos) const;
  void set_element(size_type pos, const_reference value);
  void push_back(const_reference value);
  void clear();

  iterator insert(iterator pos, const_reference value);
  template <typename... Args>
  iterator insert_many(iterator pos, Args&&... args);
  template <typename... Args>
  void insert_many_back(Args&&... args);

  void erase(iterator pos);
  void pop_back();
  void swap(Vector<T>& other);

 protected:
  void allocate_vector(size_type size);

 private:
  std::shared_ptr<T[]> data_;
  size_type size_{0};
  size_type capacity_{0};
};

template <typename T>
void Vector<T>::allocate_vector(size_type size) {
  try {
    data_.reset(new T[size]);
  } catch (std::bad_alloc& e) {
    throw std::runtime_error("Error: Failed to allocate memory");
  }
}

template <typename T>
Vector<T>::Vector(size_type capacity, const_reference value)
    : size_(capacity), capacity_(capacity) {
  allocate_vector(capacity_);
  std::uninitialized_fill_n(data_.get(), capacity_, value);
}

template <typename T>
Vector<T>::Vector(std::initializer_list<T> const& items)
    : size_(items.size()), capacity_(items.size()) {
  allocate_vector(size_);
  std::copy(items.begin(), items.end(), data_.get());
}

template <typename T>
Vector<T>::Vector(const Vector<T>& v) : size_(v.size()), capacity_(v.size()) {
  allocate_vector(size_);
  std::copy(v.begin(), v.end(), data_.get());
}

template <typename T>
Vector<T>::Vector(Vector<T>&& v) noexcept
    : data_(std::move(v.data_)), size_(v.size()), capacity_(v.size()) {
  v.size_ = 0;
  v.capacity_ = 0;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector&& v) noexcept {
  size_ = v.size();
  capacity_ = v.capacity();
  allocate_vector(size_);
  std::copy(v.begin(), v.end(), data_.get());

  return *this;
}

template <typename T>
bool Vector<T>::empty() const noexcept {
  return size_ == 0;
}

template <typename T>
typename Vector<T>::size_type Vector<T>::size() const noexcept {
  return size_;
}

template <typename T>
typename Vector<T>::size_type Vector<T>::max_size() const noexcept {
  return std::numeric_limits<size_type>::max();
}

template <typename T>
typename Vector<T>::size_type Vector<T>::capacity() const noexcept {
  return capacity_;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::begin() {
  return iterator(data_, size_);
}

template <typename T>
typename Vector<T>::iterator Vector<T>::end() {
  iterator b = begin();
  return b + size_;
}

template <typename T>
typename Vector<T>::const_iterator Vector<T>::begin() const {
  return const_iterator(data_, size_);
}

template <typename T>
typename Vector<T>::const_iterator Vector<T>::end() const {
  const_iterator b = begin();
  return b + size_;
}

template <typename T>
typename Vector<T>::const_iterator Vector<T>::cbegin() const {
  return const_iterator(data_, size_);
}

template <typename T>
typename Vector<T>::const_iterator Vector<T>::cend() const {
  const_iterator b = cbegin();
  return b + size_;
}

template <typename T>
void Vector<T>::reserve(size_type new_cap) {
  if (!new_cap) new_cap = 2;

  if (new_cap > capacity()) {
    auto tmp{*this};
    capacity_ = new_cap;
    allocate_vector(capacity_);
    std::copy(tmp.begin(), tmp.end(), data_.get());
  }
}

template <typename T>
typename Vector<T>::reference Vector<T>::at(const size_type pos) {
  if (pos >= size_) {
    throw std::out_of_range("Error: Attempt to access beyond the vector");
  }
  iterator it = begin() + pos;

  return *it;
}

template <typename T>
typename Vector<T>::const_reference Vector<T>::at(const size_type pos) const {
  if (pos >= size_) {
    throw std::out_of_range("Error: Attempt to access beyond the vector");
  }
  const_iterator it = begin() + pos;

  return *it;
}

template <typename T>
typename Vector<T>::reference Vector<T>::operator[](size_type pos) {
  return at(pos);
}

template <typename T>
typename Vector<T>::const_reference Vector<T>::operator[](size_type pos) const {
  return at(pos);
}

template <typename T>
void Vector<T>::set_element(size_type pos, const_reference value) {
  this->at(pos) = value;
}

template <typename T>
void Vector<T>::push_back(const_reference value) {
  insert_many_back(value);
}

template <typename T>
typename Vector<T>::const_reference Vector<T>::front() const {
  return at(0);
}

template <typename T>
typename Vector<T>::const_reference Vector<T>::back() const {
  return at(size_ - 1);
}

template <typename T>
void Vector<T>::shrink_to_fit() {
  if (size_ < capacity_) {
    capacity_ = size_;
    std::shared_ptr<T[]> tmp(new T[capacity_]);
    std::copy(data_.get(), data_.get() + size_, tmp.get());
    data_ = std::move(tmp);
  }
}

template <typename T>
void Vector<T>::clear() {
  size_ = 0;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::insert(iterator pos,
                                               const_reference value) {
  return insert_many(pos, value);
}

template <typename T>
void Vector<T>::erase(iterator pos) {
  size_t posIndex = std::distance(begin(), pos);
  auto it = begin();
  for (size_t i = posIndex; i < size() - 1; ++i) {
    it[i] = it[i + 1];
  }
  --size_;
}

template <typename T>
void Vector<T>::pop_back() {
  if (size_ > 0) {
    --size_;
  }
}

template <typename T>
void Vector<T>::swap(Vector<T>& other) {
  std::swap(data_, other.data_);
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
}

template <typename T>
template <typename... Args>
typename Vector<T>::iterator Vector<T>::insert_many(iterator pos,
                                                    Args&&... args) {
  size_t posIndex = std::distance(begin(), pos);
  size_t numArgs = sizeof...(args);

  if (size() + numArgs > capacity()) {
    reserve(size() + numArgs); 
    pos = begin() + posIndex;  
  }

  auto it = begin() + posIndex;
  for (size_t i = size() + numArgs - 1; i >= posIndex + numArgs; --i) {
    it[i] = it[i - numArgs];
  }

  ((it[posIndex++] = std::forward<Args>(args)), ...);

  size_ += numArgs;

  return pos;
}

template <typename T>
template <typename... Args>
void Vector<T>::insert_many_back(Args&&... args) {
  if (size() == capacity()) {
    reserve(capacity() * 2);
  }
  ((data_[size_++] = std::forward<Args>(args)), ...);
}

}
