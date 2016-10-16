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

    inline void generateKeyToValueTransitiveMap(Table<K, V> table) {
        std::set<K> keys = table.getKeys();

        if (keys.empty()) {
            return;
        }

        /* Perform DFS on the table to get transitive table. */
        for (const auto &key : keys) {
            std::stack<K> keyStack;
            keyStack.push(key);

            std::set<K> visited;
            while (!keyStack.empty()) {
                std::set<V> values = table.getValues(keyStack.top());

                keyStack.pop();

                /* End of the transitive closure for the key. */
                if (values.empty()) {
                    continue;
                }

                keyToValueTransitiveMap[key].insert(values.begin(), values.end());

                for (const auto &value : values) {
                    if (visited.count(value) != 1) {
                        keyStack.push(value);
                        visited.insert(value);
                    }
                }
            }
        }
    }

    inline void generateValueToKeyTransitiveMap(Table<K, V> table) {
        std::set<V> values = table.getValues();

        if (values.empty()) {
            return;
        }

        /* Perform DFS on the table to get transitive table. */
        for (const auto &value : values) {
            std::stack<V> valueStack;
            valueStack.push(value);

            std::set<V> visited;
            while (!valueStack.empty()) {
                std::set<K> keys = table.getKeys(valueStack.top());

                valueStack.pop();

                /* End of the transitive closure for the key. */
                if (keys.empty()) {
                    continue;
                }

                valueToKeyTransitiveMap[value].insert(keys.begin(), keys.end());

                for (const auto &key : keys) {
                    if (visited.count(key) != 1) {
                        valueStack.push(key);
                        visited.insert(key);
                    }
                }
            }
        }
    }

    inline std::set<K> getKeys(V value) {
        /* If does not exist, return empty set. */
        if (!hasValue(value)) {
            return std::set<K>();
        }

        return valueToKeyTransitiveMap[value];
    }

    inline std::set<V> getValues(K key) {
        /* If does not exist, return empty set. */
        if (!hasKey(key)) {
            return std::set<V>();
        }

        return keyToValueTransitiveMap[key];
    }

    inline unsigned int getNumberOfValues() {
        unsigned int numberOfValues = 0;
        for (const auto &pair : keyToValueTransitiveMap) {
            numberOfValues += pair.second.size();
        }

        return numberOfValues;
    }

    inline bool hasKey(K key) {
        return (keyToValueTransitiveMap.count(key) == 1);
    }

    inline bool hasValue(V value) {
        return (valueToKeyTransitiveMap.count(value) == 1);
    }

    inline bool hasKeyToValue(K key, V value) {
        std::set<V> values = keyToValueTransitiveMap[key];

        return (values.count(value) == 1);
    }

    inline bool hasKeyToValues(K key, std::set<V> subvalues) {
        std::set<V> values = keyToValueTransitiveMap[key];

        for (auto &value : subvalues) {
            if (values.count(value) != 1) {
                return false;
            }
        }

        return true;
    }

    inline bool hasValueToKey(K key, V value) {
        std::set<K> keys = valueToKeyTransitiveMap[value];

        return (keys.count(key) == 1);
    }

    inline bool hasValueToKeys(std::set<K> subkeys, V value) {
        std::set<K> keys = valueToKeyTransitiveMap[value];

        for (auto &key : subkeys) {
            if (keys.count(key) != 1) {
                return false;
            }
        }

        return true;
    }

    inline void printTable() {
        for (const auto &pair : keyToValueTransitiveMap) {
            std::cout << pair.first << " -> { ";

            for (const auto &value : pair.second) {
                std::cout << value << " ";
            }

            std::cout << "}" << std::endl;
        }

        std::cout << "=====================" << std::endl;

        for (const auto &pair : valueToKeyTransitiveMap) {
            std::cout << pair.first << " -> { ";

            for (const auto &key : pair.second) {
                std::cout << key << " ";
            }

            std::cout << "}" << std::endl;
        }
    }

    inline void clear() {
        keyToValueTransitiveMap.clear();
        valueToKeyTransitiveMap.clear();
    }

 private:
    std::unordered_map<K, std::set<V>> keyToValueTransitiveMap;
    std::unordered_map<V, std::set<K>> valueToKeyTransitiveMap;
};
