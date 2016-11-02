#pragma once

#include <iostream>
#include <set>
#include <vector>
#include <unordered_map>
#include <utility>

template<typename K, typename V>
class VectorTable {
 public:
    inline VectorTable<K, V>() {
        numberOfRelationship = 0;

        keyToValuesMap.clear();
        valueToKeysMap.clear();
    }

    inline ~VectorTable<K, V>() {}

    inline void insert(K key, V value) {
        if (!hasKeyToValue(key, value)) {
            keyToValuesMap[key].push_back(value);
            valueToKeysMap[value].push_back(key);

            numberOfRelationship++;
        }
    }

    inline std::vector<K> getKeys() {
        std::vector<K> keys;
        for (const auto &pair : keyToValuesMap) {
            keys.push_back(pair.first);
        }

        return keys;
    }

    inline std::vector<K> getKeys(V value) {
        /* If does not exist, return empty set. */
        if (!hasValue(value)) {
            return std::vector<K>();
        }

        return valueToKeysMap.at(value);
    }

    inline std::vector<V> getValues() {
        std::vector<V> values;
        for (const auto &pair : valueToKeysMap) {
            values.push_back(pair.first);
        }

        return values;
    }

    inline std::vector<V> getValues(K key) {
        /* If does not exist, return empty set. */
        if (!hasKey(key)) {
            return std::vector<V>();
        }

        return keyToValuesMap.at(key);
    }

    inline unsigned int getNumberOfRelationship() {
        return numberOfRelationship;
    }

    inline bool hasKey(K key) {
        return (keyToValuesMap.count(key) == 1);
    }

    inline bool hasValue(V value) {
        return (valueToKeysMap.count(value) == 1);
    }

    inline bool hasKeyToValue(K key, V value) {
        std::vector<V> values = keyToValuesMap[key];

        return (std::find(values.begin(), values.end(), value) != values.end());
    }

    inline bool hasValueToKey(K key, V value) {
        std::set<K> keys = valueToKeysMap.at(value);

        return (std::find(keys.begin(), keys.end(), key) != keys.end());
    }

    inline void print() {
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

 private:
    unsigned int numberOfRelationship;

    std::unordered_map<K, std::vector<V>> keyToValuesMap;
    std::unordered_map<V, std::vector<K>> valueToKeysMap;
};
