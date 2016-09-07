#pragma once

#include <map>
#include <vector>

template <class K, class V>
class Table {
 public:
  inline Table<K, V>() {
      typedef std::multimap<K, V>::iterator iterator;
  }

  inline ~Table<K, V>() {}

  inline void insert(K key, V value) {
      map.insert(std::pair<K, V>(key, value));
      reversedMap.insert(std::pair<V, K>(value, key));
  }

  inline std::vector<V> getValues(K key) {
      std::vector<K> result;

      std::pair<iterator, iterator> range = map.equal_range(key);
      for (iterator iter = range.first; iter != range.second; iter++) {
          result.push_back(iter->second);
      }

      return result;
  }

  inline std::vector<K> getKeys(V value) {
      std::vector<K> result;
      
      std::pair<iterator, iterator> range = reversedMap.equal_range(value);
      for (iterator iter = range.first; iter != range.second; iter++) {
          result.push_back(iter->second);
      }

      return result;
  }

  // TODO(pixelducky): update function
  /*std::vector<K> Table<K, V>::GetKey(std::string value) {
    /*std::vector<Z> keys;
    std::pair<MapType::iterator, MapType::iterator> aRange = reverse_map.equal_range(value);

    std::transform(aRange.first, aRange.second, std::back_inserter(v), [] (std::pair<std::string, int> element) {return element.second; });
    return keys
    return vector<K>();
  }*/

 private:
    std::multimap<K, V> map;
    std::multimap<V, K> reversedMap;
};
