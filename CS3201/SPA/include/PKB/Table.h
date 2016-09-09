#pragma once

#include <iostream>
#include <map>
#include <set>

template<typename K, typename V>
class Table {
 public:
    inline Table<K, V>() {}
    inline ~Table<K, V>() {}

    inline void insert(K key, V value) {
        keyToValueMap[key].insert(value);
        valueToKeyMap[value].insert(key);
    }
    
    inline void insert(K key, std::set<V> values) {
        keyToValueMap.insert(std::pair<K, std::set<V>>(key, values));

        for (auto &value : values) {
            valueToKeyMap[value].insert(key);
        }
    }

    inline void insert(std::set<K> keys, V value) {
        valueToKeyMap.insert(std::pair<V, std::set<K>>(value, keys));

        for (auto &key : keys) {
            keyToValueMap[key].insert(value);
        }
    }

    inline bool hasKeyValue(K key, V value) {
        std::set<V> values = keyToValueMap[key];

        return (find(values.begin(), values.end(), value) != values.end());
    }

    inline std::set<K> getKeys(V value) {
        /* If does not exist, return empty set. */
        if (valueToKeyMap.find(value) == valueToKeyMap.end()) {
            return std::set<K>();
        }

        return valueToKeyMap[value];
    }

    inline std::set<V> getValues(K key) {
        /* If does not exist, return empty set. */
        if (keyToValueMap.find(key) == keyToValueMap.end()) {
            return std::set<V>();
        }

        return keyToValueMap[key];
    }

    inline void printTable() {
        for (const auto &pair : keyToValueMap) {
            std::cout << pair.first << " -> { ";

            for (const auto &value : pair.second) {
                std::cout << value << " ";
            }

            std::cout << "}" << std::endl;
        }
    }

 private:
    std::map<K, std::set<V>> keyToValueMap;
    std::map<V, std::set<K>> valueToKeyMap;
};
