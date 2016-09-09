#pragma once

#include <iostream>
#include <map>
#include <set>
#include <stack>

template<typename K, typename V>
class TransitiveTable {
 public:
    inline TransitiveTable<K, V>() {}
    inline ~TransitiveTable<K, V>() {}

    inline void generateKeyToValueTransitiveMap(Table<unsigned int, unsigned int> table) {
        if (table.getKeyToValueMap().empty()) {
            return;
        }

        for (const auto &pair : table.getKeyToValueMap()) {
            std::stack<K> keyStack;
            keyStack.push(pair.first);

            while (!keyStack.empty()) {
                std::set<V> values = table.getValues(keyStack.top());

                keyStack.pop();

                /* End of the transitive closure for the key. */
                if (values.empty()) {
                    continue;
                }

                keyToValueTransitiveMap[pair.first].insert(values.begin(), values.end());

                for (const auto &value : values) {
                    keyStack.push(value);
                }
            }
        }
    }

    inline void generateValueToKeyTransitiveMap(Table<unsigned int, unsigned int> table) {
        if (table.getValueToKeyMap().empty()) {
            return;
        }

        for (const auto &pair : table.getValueToKeyMap()) {
            std::stack<K> valueStack;
            valueStack.push(pair.first);

            while (!valueStack.empty()) {
                std::set<V> keys = table.getKeys(valueStack.top());

                valueStack.pop();

                /* End of the transitive closure for the key. */
                if (keys.empty()) {
                    continue;
                }

                valueToKeyTransitiveMap[pair.first].insert(keys.begin(), keys.end());

                for (const auto &key : keys) {
                    valueStack.push(key);
                }
            }
        }
    }

    inline void printTransitiveTable() {
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

 private:
    std::map<K, std::set<V>> keyToValueTransitiveMap;
    std::map<V, std::set<K>> valueToKeyTransitiveMap;
};