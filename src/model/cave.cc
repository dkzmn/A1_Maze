#include "cave.h"

#include <exception>
#include <fstream>

namespace s21 {

void Cave::Init(size_t width, size_t height, double chance) {
  if (width < 1 || width > 50 || height < 1 || height > 50) {
    throw std::out_of_range("Wrong cave size.");
  }
  width_ = width;
  height_ = height;
  matrix_.resize(height_ + 2);
  for (size_t i = 0; i < height_ + 2; ++i) {
    matrix_[i].resize(width_ + 2);
    for (size_t j = 0; j < width_ + 2; ++j) {
      if (i == 0 || j == 0 || j == width_ + 1 || i == height_ + 1) {
        matrix_[i][j] = true;
      } else {
        matrix_[i][j] = (bool)(rand() < chance * (RAND_MAX + 1.0));
      }
    }
  }
}

void Cave::LoadFromFile(std::string filename) {
  std::fstream myfile(filename);
  size_t height, width, tmp;
  myfile >> width >> height;
  if (width < 1 || width > 50 || height < 1 || height > 50) {
    throw std::out_of_range("Wrong cave size.");
  }
  width_ = width;
  height_ = height;
  matrix_.resize(height_ + 2);
  for (size_t i = 0; i < height_ + 2; ++i) {
    matrix_[i].resize(width_ + 2);
    for (size_t j = 0; j < width_ + 2; ++j) {
      if (i == 0 || j == 0 || j == width_ + 1 || i == height_ + 1) {
        matrix_[i][j] = true;
      } else {
        if (!myfile.eof()) {
          myfile >> tmp;
          matrix_[i][j] = (bool)tmp;
        } else {
          matrix_.resize(0);
          width_ = 0;
          height_ = 0;
          throw std::length_error("Not enought data in file");
        }
      }
    }
  }
}

void Cave::NextStep(int birth_limit, int death_limit) noexcept {
  std::vector<std::vector<bool>> tmp(matrix_);
  for (size_t i = 1; i <= height_; ++i) {
    for (size_t j = 1; j <= width_; ++j) {
      int live_neigh = matrix_[i - 1][j - 1] + matrix_[i - 1][j] +
                       matrix_[i - 1][j + 1] + matrix_[i][j - 1] +
                       matrix_[i][j + 1] + matrix_[i + 1][j - 1] +
                       matrix_[i + 1][j] + matrix_[i + 1][j + 1];
      if (matrix_[i][j] && live_neigh < death_limit) {
        tmp[i][j] = false;
      }
      if (!matrix_[i][j] && live_neigh > birth_limit) {
        tmp[i][j] = true;
      }
    }
    matrix_ = tmp;
  }
}

std::vector<std::vector<bool>> Cave::getCave() const noexcept {
  return matrix_;
}

size_t Cave::getWidth() const noexcept { return width_; }

size_t Cave::getHeight() const noexcept { return height_; }

}  // namespace s21
