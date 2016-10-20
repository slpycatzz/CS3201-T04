#pragma once

#include <iostream>
#include <set>
#include <stack>
#include <unordered_map>

template<typename K, typename V>
class TransitiveTable {
 public:
    inline TransitiveTable<K, V>() {}
    inline ~TransitiveTable<K, V>() {}

    inline void generateKeyToValuesTransitiveMap(Table<K, V> table) {
        std::set<K> keys = table.getKeys();

        if (keys.empty()) {
            return;
        }

        /* Perform DFS on the table to get transitive table. */
        for (const auto &key : keys) {
            std::stack<K> keyStack;
            keyStack.push(key);

            while (!keyStack.empty()) {
                std::set<V> values = table.getValues(keyStack.top());

                keyStack.pop();

                /* End of the transitive closure for the key. */
                if (values.empty()) {
                    continue;
                }

                keyToValuesTransitiveMap[key].insert(values.begin(), values.end());

                for (const auto &value : values) {
                    keyStack.push(value);
                }
            }
        }
    }

    inline void generateValueToKeysTransitiveMap(Table<K, V> table) {
        std::set<V> values = table.getValues();

        if (values.empty()) {
            return;
        }

        /* Perform DFS on the table to get transitive table. */
        for (const auto &value : values) {
            std::stack<V> valueStack;
            valueStack.push(value);

            while (!valueStack.empty()) {
                std::set<K> keys = table.getKeys(valueStack.top());

                valueStack.pop();

                /* End of the transitive closure for the key. */
                if (keys.empty()) {
                    continue;
                }

                valueToKeysTransitiveMap[value].insert(keys.begin(), keys.end());

                for (const auto &key : keys) {
                    valueStack.push(key);
                }
            }
        }
    }

    inline void insert(K key, V value) {
        keyToValuesTransitiveMap[key].insert(value);
        valueToKeysTransitiveMap[value].insert(key);
    }

    inline std::set<K> getKeys(V value) {
        /* If does not exist, return empty set. */
        if (!hasValue(value)) {
            return std::set<K>();
        }

        return valueToKeysTransitiveMap[value];
    }

    inline std::set<V> getValues(K key) {
        /* If does not exist, return empty set. */
        if (!hasKey(key)) {
            return std::set<V>();
        }

        return keyToValuesTransitiveMap[key];
    }

    inline unsigned int getNumberOfValues() {
        unsigned int numberOfValues = 0;
        for (const auto &pair : keyToValuesTransitiveMap) {
            numberOfValues += pair.second.size();
        }

        return numberOfValues;
    }

    inline bool hasKey(K key) {
        return (keyToValuesTransitiveMap.count(key) == 1);
    }

    inline bool hasValue(V value) {
        return (valueToKeysTransitiveMap.count(value) == 1);
    }

    inline bool hasKeyToValue(K key, V value) {
        std::set<V> values = keyToValuesTransitiveMap[key];

        return (values.count(value) == 1);
    }

    inline bool hasKeyToValues(K key, std::set<V> subvalues) {
        std::set<V> values = keyToValuesTransitiveMap[key];

        for (auto &value : subvalues) {
            if (values.count(value) != 1) {
                return false;
            }
        }

        return true;
    }

    inline bool hasValueToKey(K key, V value) {
        std::set<K> keys = valueToKeysTransitiveMap[value];

        return (keys.count(key) == 1);
    }

    inline bool hasValueToKeys(std::set<K> subkeys, V value) {
        std::set<K> keys = valueToKeysTransitiveMap[value];

        for (auto &key : subkeys) {
            if (keys.count(key) != 1) {
                return false;
            }
        }

        return true;
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

    inline void clear() {
        keyToValuesTransitiveMap.clear();
        valueToKeysTransitiveMap.clear();
    }

 private:
    std::unordered_map<K, std::set<V>> keyToValuesTransitiveMap;
    std::unordered_map<V, std::set<K>> valueToKeysTransitiveMap;
};
