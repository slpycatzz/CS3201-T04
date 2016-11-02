#pragma once

#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

template<typename K, typename V>
class Table {
 public:
    inline Table<K, V>() {
        keyToValueMap.clear();
        valueToKeyMap.clear();
    }

    inline ~Table<K, V>() {}

    inline void insert(K key, V value) {
        if (!hasKeyToValue(key, value)) {
            keyToValueMap[key] = value;
            valueToKeyMap[value] = key;
        }
    }

    inline K getKey(V value) {
        return valueToKeyMap.at(value);
    }

    inline std::vector<K> getKeys() {
        std::vector<K> keys;
        for (const auto &pair : keyToValueMap) {
            keys.push_back(pair.first);
        }

        return keys;
    }

    inline V getValue(K key) {
        return keyToValueMap.at(key);
    }

    inline std::vector<V> getValues() {
        std::vector<V> values;
        for (const auto &pair : valueToKeyMap) {
            values.push_back(pair.first);
        }

        return values;
    }

    inline bool hasKey(K key) {
        return (keyToValueMap.count(key) == 1);
    }

    inline bool hasValue(V value) {
        return (valueToKeyMap.count(value) == 1);
    }

    inline bool hasKeyToValue(K key, V value) {
        return (keyToValueMap[key] == value);
    }

    inline bool hasValueToKey(K key, V value) {
        return (valueToKeyMap[value] == key);
    }

    inline void print() {
        for (const auto &pair : keyToValueMap) {
            std::cout << pair.first << " -> { ";
            std::cout << pair.second << " }" << std::endl;
        }

        std::cout << "=====================" << std::endl;

        for (const auto &pair : valueToKeyMap) {
            std::cout << pair.first << " -> { ";
            std::cout << pair.second << " }" << std::endl;
        }
    }

 private:
    std::unordered_map<K, V> keyToValueMap;
    std::unordered_map<V, K> valueToKeyMap;
};
