#pragma once

#include <iostream>
#include <map>
#include <set>
#include <utility>

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
        keyToValueMap.insert(std::make_pair(key, values));

        for (auto &value : values) {
            valueToKeyMap[value].insert(key);
        }
    }

    inline void insert(std::set<K> keys, V value) {
        valueToKeyMap.insert(std::make_pair(value, keys));

        for (auto &key : keys) {
            keyToValueMap[key].insert(value);
        }
    }

    inline K getKey(V value) {
        /* If does not exist, return empty set. */
        if (!hasValue(value)) {
            return NULL;
        }

        return *(valueToKeyMap[value].begin());
    }

    inline std::set<K> getKeys(V value) {
        /* If does not exist, return empty set. */
        if (!hasValue(value)) {
            return std::set<K>();
        }

        return valueToKeyMap[value];
    }

    inline V getValue(K key) {
        /* If does not exist, return empty set. */
        if (!hasKey(key)) {
            return NULL;
        }

        return *(keyToValueMap[key].begin());
    }

    inline std::set<V> getValues() {
        std::set<V> values;
        for (const auto &pair : keyToValueMap) {
            values.insert(pair.second.begin(), pair.second.end());
        }

        return values;
    }

    inline std::set<V> getValues(K key) {
        /* If does not exist, return empty set. */
        if (!hasKey(key)) {
            return std::set<V>();
        }

        return keyToValueMap[key];
    }

    inline std::map<K, std::set<V>> getKeyToValueMap() {
        return keyToValueMap;
    }

    inline std::map<V, std::set<K>> getValueToKeyMap() {
        return valueToKeyMap;
    }

    inline bool hasKey(K key) {
        return (keyToValueMap.find(key) != keyToValueMap.end());
    }

    inline bool hasValue(V value) {
        return (valueToKeyMap.find(value) != valueToKeyMap.end());
    }

    inline bool hasKeyToValue(K key, V value) {
        std::set<V> values = keyToValueMap[key];

        return (find(values.begin(), values.end(), value) != values.end());
    }

    inline bool hasValueToKey(K key, V value) {
        std::set<K> keys = valueToKeyMap[value];

        return (find(keys.begin(), keys.end(), key) != keys.end());
    }

    inline void printTable() {
        for (const auto &pair : keyToValueMap) {
            std::cout << pair.first << " -> { ";

            for (const auto &value : pair.second) {
                std::cout << value << " ";
            }

            std::cout << "}" << std::endl;
        }

        std::cout << "=====================" << std::endl;

        for (const auto &pair : valueToKeyMap) {
            std::cout << pair.first << " -> { ";

            for (const auto &key : pair.second) {
                std::cout << key << " ";
            }

            std::cout << "}" << std::endl;
        }
    }

 private:
    std::map<K, std::set<V>> keyToValueMap;
    std::map<V, std::set<K>> valueToKeyMap;
};
