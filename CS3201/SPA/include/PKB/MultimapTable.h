#pragma once

#include <iostream>
#include <map>
#include <vector>

template<typename K, typename V>
class MultimapTable {
 public:
    typedef typename std::multimap<K, V>::iterator iterator;
    typedef typename std::multimap<V, K>::iterator rIterator;

    inline MultimapTable<K, V>() {}
    inline ~MultimapTable<K, V>() {}

    inline void insert(K key, V value) {
        map.emplace(key, value);
        reversedMap.emplace(value, key);
    }

    inline std::vector<V> getValues(K key) {
        std::vector<V> result;

        std::pair<iterator, iterator> range = map.equal_range(key);
        for (iterator iter = range.first; iter != range.second; iter++) {
            result.push_back(iter->second);
        }

        return result;
    }

    inline std::vector<K> getKeys(V value) {
        std::vector<K> result;
      
        std::pair<rIterator, rIterator> range = reversedMap.equal_range(value);
        for (rIterator iter = range.first; iter != range.second; iter++) {
            result.push_back(iter->second);
        }

        return result;
    }

    inline void printTable() {
        for (iterator iter = map.begin(); iter != map.end(); iter++) {
            std::cout << iter->first << "->";
            std::cout << iter->second << std::endl;
        }
    }

 private:
    std::multimap<K, V> map;
    std::multimap<V, K> reversedMap;
};
