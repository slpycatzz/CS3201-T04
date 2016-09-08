#pragma once

#include <iostream>
#include <vector>

template<typename V>
class VectorTable {
 public:
    inline VectorTable<V>() {}
    inline ~VectorTable<V>() {}

    inline void insert(V value) {
        values.push_back(value);
    }

    inline int getSize() {
        return values.size();
    }

    inline bool hasValue(V value) {
        return (find(values.begin(), values.end(), value) != values.end());
    }

    inline void printTable() {
        for (unsigned int i = 0; i < values.size(); i++) {
            std::cout << (i + 1) << ". ";
            std::cout << values[i] << std::endl;
        }
    }

 private:
    std::vector<V> values;
};
