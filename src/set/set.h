#include "hash_table.h"

namespace containers {

template <typename K, typename H = std::hash<K>>
class Set {
 public:
  using table = hash_table<K, K, H>;
  using key_type = K;
  using mapped_type = K;
  using value_type = std::pair<key_type, mapped_type>;
  using reference = value_type&;
  using iterator = typename table::iterator;
  using size_type = size_t;

  Set() = default;

  Set(std::initializer_list<mapped_type> const& items) {
    for (auto& it : items) {
      t[it] = it;
    }
  }

  Set(const Set& other) = default;
  Set(Set&& other) = default;
  ~Set() noexcept = default;

  Set& operator=(const Set& other) = default;
  Set& operator=(Set&& other) noexcept = default;

  iterator begin() { return t.begin(); }
  iterator end() { return t.end(); }

  bool empty() const noexcept { return t.empty(); }
  size_type size() const noexcept { return t.size(); }

  void clear() { t.clear(); }
  std::pair<iterator, bool> insert(const mapped_type& value) {
    value_type p = {value, value};
    return t.insert(p);
  }
  void erase(iterator pos) { t.erase(pos); }
  void swap(Set& other) { t.swap(other.t); }

  iterator find(const key_type& key) { return t.find(key); }
  bool contains(const key_type& key) const noexcept { return t.contains(key); }
  template <typename... Args>
  containers::Vector<std::pair<iterator, bool>> insert_many(Args&&... args) {
    return {insert(std::forward<Args>(args))...};
  }

 private:
  table t;
};

}
