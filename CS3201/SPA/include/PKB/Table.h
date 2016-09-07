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
        map.emplace(key, value);
        reversedMap.emplace(value, key);
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

 private:
    std::multimap<K, V> map;
    std::multimap<V, K> reversedMap;
};
