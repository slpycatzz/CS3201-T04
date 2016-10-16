#pragma once

#include <iostream>
#include <set>
#include <unordered_map>
#include <utility>

template<typename K, typename V>
class Table {
 public:
    inline Table<K, V>() {}
    inline ~Table<K, V>() {}

    inline void insert(K key, V value) {
        keyToValuesMap[key].insert(value);
        valueToKeysMap[value].insert(key);
    }

    inline void insert(K key, std::set<V> values) {
        keyToValuesMap.insert(std::make_pair(key, values));

        for (auto &value : values) {
            valueToKeysMap[value].insert(key);
        }
    }

    inline void insert(std::set<K> keys, V value) {
        valueToKeysMap.insert(std::make_pair(value, keys));

        for (auto &key : keys) {
            keyToValuesMap[key].insert(value);
        }
    }

    inline K getKey(V value) {
        if (!hasValue(value)) {
            return NULL;
        }

        return *(valueToKeysMap[value].begin());
    }

    inline std::set<K> getKeys() {
        std::set<K> keys;
        for (const auto &pair : valueToKeysMap) {
            keys.insert(pair.second.begin(), pair.second.end());
        }

        return keys;
    }

    inline std::set<K> getKeys(V value) {
        /* If does not exist, return empty set. */
        if (!hasValue(value)) {
            return std::set<K>();
        }

        return valueToKeysMap[value];
    }

    inline V getValue(K key) {
        if (!hasKey(key)) {
            return NULL;
        }

        return *(keyToValuesMap[key].begin());
    }

    inline std::set<V> getValues() {
        std::set<V> values;
        for (const auto &pair : keyToValuesMap) {
            values.insert(pair.second.begin(), pair.second.end());
        }

        return values;
    }

    inline std::set<V> getValues(K key) {
        /* If does not exist, return empty set. */
        if (!hasKey(key)) {
            return std::set<V>();
        }

        return keyToValuesMap[key];
    }

    inline std::unordered_map<K, std::set<V>> getKeyToValuesMap() {
        return keyToValuesMap;
    }

    inline std::unordered_map<V, std::set<K>> getValueToKeysMap() {
        return valueToKeysMap;
    }

    inline unsigned int getNumberOfValues() {
        unsigned int numberOfValues = 0;
        for (const auto &pair : keyToValuesMap) {
            numberOfValues += pair.second.size();
        }

        return numberOfValues;
    }

    inline bool hasKey(K key) {
        return (keyToValuesMap.count(key) == 1);
    }

    inline bool hasValue(V value) {
        return (valueToKeysMap.count(value) == 1);
    }

    inline bool hasKeyToValue(K key, V value) {
        std::set<V> values = keyToValuesMap[key];

        return (values.count(value) == 1);
    }

    inline bool hasKeyToValues(K key, std::set<V> subvalues) {
        std::set<V> values = keyToValuesMap[key];

        for (auto &value : subvalues) {
            if (values.count(value) != 1) {
                return false;
            }
        }

        return true;
    }

    inline bool hasValueToKey(K key, V value) {
        std::set<K> keys = valueToKeysMap[value];

        return (keys.count(key) == 1);
    }

    inline bool hasValueToKeys(std::set<K> subkeys, V value) {
        std::set<K> keys = valueToKeysMap[value];

        for (auto &key : subkeys) {
            if (keys.count(key) != 1) {
                return false;
            }
        }

        return true;
    }

    inline void printTable() {
        for (const auto &pair : keyToValuesMap) {
            std::cout << pair.first << " -> { ";

            for (const auto &value : pair.second) {
                std::cout << value << " ";
            }

            std::cout << "}" << std::endl;
        }

        std::cout << "=====================" << std::endl;

        for (const auto &pair : valueToKeysMap) {
            std::cout << pair.first << " -> { ";

            for (const auto &key : pair.second) {
                std::cout << key << " ";
            }

            std::cout << "}" << std::endl;
        }
    }

    inline void clear() {
        keyToValuesMap.clear();
        valueToKeysMap.clear();
    }

 private:
    std::unordered_map<K, std::set<V>> keyToValuesMap;
    std::unordered_map<V, std::set<K>> valueToKeysMap;
};
