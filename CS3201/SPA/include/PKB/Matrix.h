#pragma once

#include <vector>

#include "PKB/PKB.h"

class Matrix {
 public:
    inline Matrix() {
        size_ = 0;

        rowPopulated_ = std::vector<unsigned char>();
        matrix_ = std::vector<std::vector<unsigned char>>();
    }

    inline Matrix(unsigned int size) {
        size_ = size;

        rowPopulated_.resize(size_, 0);
        matrix_.resize(size_, std::vector<unsigned char>(size_, 0));
    }

    inline ~Matrix() {}

    inline void toggleRowColumn(unsigned int row, unsigned int column) {
        matrix_[row][column] = 1;
    }

    inline bool isRowColumnToggled(unsigned int row, unsigned int column) {
        return (matrix_[row][column] == 1);
    }

    inline void setPopulated(unsigned int row) {
        rowPopulated_[row] = 1;
    }

    inline bool isRowPopulated(unsigned int row) {
        return (rowPopulated_[row] == 1);
    }

    inline unsigned int getSize() {
        return size_;
    }

    inline void clear() {
        rowPopulated_.resize(size_, 0);
        matrix_.resize(size_, std::vector<unsigned char>(size_, 0));
    }

 private:
    unsigned int size_;

    std::vector<unsigned char> rowPopulated_;
    std::vector<std::vector<unsigned char>> matrix_;
};

