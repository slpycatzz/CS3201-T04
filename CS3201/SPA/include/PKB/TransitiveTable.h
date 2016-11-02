#pragma once

#include <iostream>
#include <stack>
#include <unordered_map>
#include <vector>

#include "VectorTable.h"

template<typename K, typename V>
class TransitiveTable {
 public:
    inline TransitiveTable<K, V>() {
        numberOfRelationship = 0;

        keyToValuesTransitiveMap.clear();
        valueToKeysTransitiveMap.clear();
    }

    inline ~TransitiveTable<K, V>() {}

    inline void generateTransitiveTable(VectorTable<K, V> table) {
        generateKeyToValuesTransitiveMap(table);
        generateValueToKeysTransitiveMap(table);
    }

    inline void generateKeyToValuesTransitiveMap(VectorTable<K, V> table) {
        std::vector<K> keys = table.getKeys();

        if (keys.empty()) {
            return;
        }

        /* Perform DFS on the table to get transitive table. */
        for (const auto &key : keys) {
            std::stack<K> keyStack;
            keyStack.push(key);

            while (!keyStack.empty()) {
                std::vector<V> values = table.getValues(keyStack.top());

                keyStack.pop();

                /* End of the transitive closure for the key. */
                if (values.empty()) {
                    continue;
                }

                keyToValuesTransitiveMap[key].insert(keyToValuesTransitiveMap[key].end(), values.begin(), values.end());
                numberOfRelationship += values.size();

                for (const auto &value : values) {
                    keyStack.push(value);
                }
            }
        }
    }

    inline void generateValueToKeysTransitiveMap(VectorTable<K, V> table) {
        std::vector<V> values = table.getValues();

        if (values.empty()) {
            return;
        }

        /* Perform DFS on the table to get transitive table. */
        for (const auto &value : values) {
            std::stack<V> valueStack;
            valueStack.push(value);

            while (!valueStack.empty()) {
                std::vector<K> keys = table.getKeys(valueStack.top());

                valueStack.pop();

                /* End of the transitive closure for the key. */
                if (keys.empty()) {
                    continue;
                }

                valueToKeysTransitiveMap[value].insert(valueToKeysTransitiveMap[value].end(), keys.begin(), keys.end());

                for (const auto &key : keys) {
                    valueStack.push(key);
                }
            }
        }
    }

    inline void insert(K key, V value) {
        keyToValuesTransitiveMap[key].insert(value);
        valueToKeysTransitiveMap[value].insert(key);

        numberOfRelationship++;
    }

    inline std::vector<K> getKeys(V value) {
        /* If does not exist, return empty set. */
        if (!hasValue(value)) {
            return std::vector<K>();
        }

        return valueToKeysTransitiveMap[value];
    }

    inline std::vector<V> getValues(K key) {
        /* If does not exist, return empty set. */
        if (!hasKey(key)) {
            return std::vector<V>();
        }

        return keyToValuesTransitiveMap[key];
    }

    inline unsigned int getNumberOfRelationship() {
        return numberOfRelationship;
    }

    inline bool isEmpty() {
        return (keyToValuesTransitiveMap.empty && valueToKeysTransitiveMap.empty);
    }

    inline bool hasKey(K key) {
        return (keyToValuesTransitiveMap.find(key) != keyToValuesTransitiveMap.end());
    }

    inline bool hasValue(V value) {
        return (valueToKeysTransitiveMap.find(value) != valueToKeysTransitiveMap.end());
    }

    inline bool hasKeyToValue(K key, V value) {
        std::vector<V> values = keyToValuesTransitiveMap[key];

        return (std::find(values.begin(), values.end(), value) != values.end());
    }

    inline void printTable() {
        for (const auto &pair : keyToValuesTransitiveMap) {
            std::cout << pair.first << " -> { ";

            for (const auto &value : pair.second) {
                std::cout << value << " ";
            }

            std::cout << "}" << std::endl;
        }

        std::cout << "=====================" << std::endl;

        for (const auto &pair : valueToKeysTransitiveMap) {
            std::cout << pair.first << " -> { ";

            for (const auto &key : pair.second) {
                std::cout << key << " ";
            }

            std::cout << "}" << std::endl;
        }
    }

 private:
    unsigned int numberOfRelationship;

    std::unordered_map<K, std::vector<V>> keyToValuesTransitiveMap;
    std::unordered_map<V, std::vector<K>> valueToKeysTransitiveMap;
};
