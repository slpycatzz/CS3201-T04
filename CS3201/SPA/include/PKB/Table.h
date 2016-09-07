#pragma once
#include <unordered_map>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>

template <typename K, typename V>
class Table {
 private:
  std::unordered_map<K, V> original_map;
  std::multimap<V, K> reverse_map;

 public:
  explicit Table<K, V>();

  // TODO(pixelducky): fix this function
  void Table<K, V>::Insert(K key, V value) {
    original_map.emplace(key, value);
    // this was giving Error C2664
    /*for (auto &i : value) {
      reverse_map.emplace(i, key);
    }*/
  }

  V Table<K, V>::GetValue(K key) {
    return original_map[key];
  }

  // TODO(pixelducky): update function
  /*std::vector<K> Table<K, V>::GetKey(std::string value) {
    /*std::vector<Z> keys;
    std::pair<MapType::iterator, MapType::iterator> aRange = reverse_map.equal_range(value);

    std::transform(aRange.first, aRange.second, std::back_inserter(v), [] (std::pair<std::string, int> element) {return element.second; });
    return keys
    return vector<K>();
  }*/
};
