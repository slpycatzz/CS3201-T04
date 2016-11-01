#pragma once

#include <vector>

#include "PKB/PKB.h"

class Matrix {
 public:
    inline Matrix() {
        populated_ = false;
        size_ = 0;

        matrix_ = std::vector<std::vector<unsigned int>>();
    }

    inline Matrix(unsigned int size) {
        populated_ = false;
        size_ = size;

        matrix_.resize(size, std::vector<unsigned int>(size, 0));
    }

    inline ~Matrix() {}

    inline void toggleRowColumn(unsigned int row, unsigned int column) {
        matrix_[row][column] = 1;
    }

    inline bool isRowColumnToggled(unsigned int row, unsigned int column) {
        return (matrix_[row][column] == 1);
    }

    inline void setPopulated(bool populated) {
        populated_ = populated;
    }

    inline bool isPopulated() {
        return populated_;
    }

    inline void clear() {
        populated_ = false;

        matrix_.clear();
    }

 private:
    bool populated_;
    unsigned int size_;

    std::vector<std::vector<unsigned int>> matrix_;
};

