#pragma once

#include <cstdlib>
#include <iostream>
#include <vector>

#include "PKB/PKB.h"

class Matrix {
 public:
    inline Matrix() {
        numberOfRelationship_ = 0;
        size_ = 0;

        rowPopulated_ = std::vector<unsigned char>();
        matrix_ = std::vector<std::vector<unsigned char>>();
    }

    inline Matrix(unsigned int size) {
        numberOfRelationship_ = 0;
        size_ = size;

        rowPopulated_.resize(size_, 0);
        matrix_.resize(size_, std::vector<unsigned char>(size_, 0));
    }

    inline ~Matrix() {}

    inline void toggleRowColumn(unsigned int row, unsigned int column) {
        matrix_[row - 1][column - 1] = 1;

        numberOfRelationship_++;
    }

    inline bool isRowColumnToggled(unsigned int row, unsigned int column) {
        return (matrix_[row - 1][column - 1] == 1);
    }

    inline void setPopulated(unsigned int row) {
        rowPopulated_[row - 1] = 1;
    }

    inline bool isRowPopulated(unsigned int row) {
        return (rowPopulated_[row - 1] == 1);
    }

    inline unsigned int getNumberOfRelationship() {
        return numberOfRelationship_;
    }

    inline unsigned int getSize() {
        return size_;
    }

    inline void clear() {
        std::memset(&rowPopulated_[0], 0, sizeof(rowPopulated_));
        std::memset(&matrix_[0][0], 0, sizeof(matrix_));
    }

    inline void print() {
        for (unsigned int i = 0; i < matrix_.size(); i++) {
            for (unsigned int k = 0; k < matrix_.size(); k++) {
                std::cout << matrix_[i][k] << " ";
            }

            std::cout << std::endl;
        }
    }

 private:
     unsigned int numberOfRelationship_;
     unsigned int size_;

    std::vector<unsigned char> rowPopulated_;
    std::vector<std::vector<unsigned char>> matrix_;
};
