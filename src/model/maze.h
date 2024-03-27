#ifndef A1_MAZE_SRC_MODEL_MAZE_H_
#define A1_MAZE_SRC_MODEL_MAZE_H_

#include <stdint.h>

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

namespace s21 {
struct Point {
  uint32_t row, col;
};
struct MazePointers {
  bool **vertical_walls;
  bool **horizontal_walls;
};
using solution = std::stack<std::pair<uint32_t, uint32_t>>;

/*!
  \brief Maze class
  Stores the maze itself, allows you to create ideal mazes and solve them.
*/

class Maze {
 public:
  // Constructors and destructor:
  Maze();
  Maze(uint32_t rows, uint32_t cols);
  ~Maze();

  // Getters:
  uint32_t GetRows() const noexcept;
  uint32_t GetCols() const noexcept;
  MazePointers GetMazeMatrices() const noexcept;
  solution GetSolution() const noexcept;
  // Main methods

  /// @brief Loads a maze from the given file with data structured in a certain
  /// way
  /// @param file_name Path to the file
  void LoadFromFile(std::string filename);
  /// @brief Saves the generated maze into .txt file
  void SaveToFile(std::string filename);
  /// @brief Generates an ideal maze, using Eller's algorithm.
  void Generate();
  /// @brief Finds a way from start to finish
  /// @param start The starting point of the way
  /// @param finish The finishing point of the way
  /// @return 1 - if the way was found, otherwise - 0
  uint32_t Solve(Point start, Point finish);

 private:
  // Helper functions
  void InitStructures();
  void FreeStructures();
  void PrepareFirstLine();
  void JoinToUniqueSet();
  void AddVerticalWalls(uint32_t row);
  void AddHorizontalWalls(uint32_t row);
  void CheckForIsolation(uint32_t row);
  bool GenerateRandomNumber();
  uint32_t CountSetMembers(uint32_t set);
  uint32_t CountWalls(uint32_t set, uint32_t row);
  void MergeSets(uint32_t pos, uint32_t set);
  void PrepareSetsForNewLine(uint32_t row);
  void GenerateLastLine();
  void CheckLastLine();
  bool WaveCanSpread(Point current);
  bool WaveCanGoUp(Point current);
  bool WaveCanGoDown(Point current);
  bool WaveCanGoLeft(Point current);
  bool WaveCanGoRight(Point current);
  uint32_t MarkCells(Point current, uint32_t counter);
  solution *TraceWay(Point start, Point finish);
  Point FindNextCell(Point current);
  // Fields
  uint32_t rows_;
  uint32_t cols_;
  bool **vertical_walls_ = nullptr;
  bool **horizontal_walls_ = nullptr;
  bool **answer_matrix_ = nullptr;
  std::vector<uint32_t> sets_;
  uint32_t counter_;
  uint32_t rand_ = time(nullptr);
  int **maze_cells_ = nullptr;
  solution *answer_ = nullptr;

  // Template for matrix creation
  template <class T>
  void CreateMatrix(T ***matrix, T value) const {
    *matrix = new T *[rows_]();
    for (uint32_t i = 0; i != rows_; ++i) {
      try {
        (*matrix)[i] = new T[cols_]();
        for (uint32_t y = 0; y != cols_; ++y) {
          (*matrix)[i][y] = value;
        }
      } catch (...) {
        for (uint32_t y = 0; y != i; ++y) {
          delete[] matrix[y];
        }
        delete[] matrix;
        throw;
      }
    }
  }

  // Template for matrix deletion
  template <class T>
  void DeleteMatrix(T ***matrix) const noexcept {
    for (uint32_t i = 0; i != rows_; ++i) {
      delete[] (*matrix)[i];
    }
    delete[] * matrix;
    matrix = nullptr;
  }

  // Template for checking that matrix is not empty
  template <class T>
  bool MatrixNotEmpty(T ***matrix) const noexcept {
    if (*matrix == nullptr)
      return 0;
    else {
      for (uint32_t i = 0; i != rows_; ++i) {
        for (uint32_t y = 0; y != cols_; ++y) {
          if (matrix[i][y] || matrix[i][y]) return 1;
        }
      }
    }
    return 0;
  }
};

}  // namespace s21

#endif  // A1_MAZE_SRC_MODEL_MAZE_H_
