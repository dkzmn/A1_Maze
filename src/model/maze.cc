#include "maze.h"

namespace s21 {
Maze::Maze() : rows_(1), cols_(1) {}

Maze::Maze(uint32_t rows, uint32_t cols) : rows_(rows), cols_(cols) {
  if (rows < 1 || cols < 1 || rows > 50 || cols > 50)
    throw std::out_of_range(
        "There should be at least one row and/or column, but no more than 50");
  InitStructures();
}

Maze::~Maze() { FreeStructures(); }

uint32_t Maze::GetRows() const noexcept { return rows_; }

uint32_t Maze::GetCols() const noexcept { return cols_; }

MazePointers Maze::GetMazeMatrices() const noexcept {
  MazePointers res;
  res.vertical_walls = vertical_walls_;
  res.horizontal_walls = horizontal_walls_;
  return res;
}

solution Maze::GetSolution() const noexcept {
  solution res;
  if (answer_) res = *answer_;
  return res;
}

void Maze::LoadFromFile(std::string file_name) {
  std::ifstream input(file_name);
  if (input) {
    uint32_t rows, cols, temp;
    input >> rows >> cols;
    if (rows < 1 || rows > 50 || cols < 1 || cols > 50)
      throw std::out_of_range(
          "There should be at least one row and/or column, but no more than "
          "50");
    rows_ = rows;
    cols_ = cols;
    InitStructures();
    for (uint32_t i = 0; i != rows_; ++i) {
      for (uint32_t y = 0; y != cols_; ++y) {
        if (!input.eof()) {
          input >> temp;
          vertical_walls_[i][y] = static_cast<bool>(temp);
        } else {
          FreeStructures();
          throw std::length_error("Not enough data in the file");
        }
      }
    }
    for (uint32_t i = 0; i != rows_; ++i) {
      for (uint32_t y = 0; y != cols_; ++y) {
        if (!input.eof()) {
          input >> temp;
          horizontal_walls_[i][y] = static_cast<bool>(temp);
        } else {
          FreeStructures();
          throw std::length_error("Not enough data in the file");
        }
      }
    }
    input.close();
  } else {
    throw std::invalid_argument("No such file.\n");
  }
}

void Maze::Generate() {
  counter_ = 1;
  for (uint32_t i = 0; i != (rows_ - 1); ++i) {
    JoinToUniqueSet();
    AddVerticalWalls(i);
    AddHorizontalWalls(i);
    CheckForIsolation(i);
    PrepareSetsForNewLine(i);
  }
  GenerateLastLine();
}

void Maze::PrepareFirstLine() {
  for (uint32_t i = 0; i < cols_; ++i) {
    sets_.push_back(0);
  }
}

void Maze::JoinToUniqueSet() {
  for (uint32_t i = 0; i != cols_; ++i) {
    if (sets_[i] == 0) {
      sets_[i] = counter_;
      ++counter_;
    }
  }
}

void Maze::MergeSets(uint32_t pos, uint32_t set) {
  uint32_t old_set = sets_[pos + 1];
  for (uint32_t i = pos; i != cols_; ++i) {
    if (sets_[i] == old_set) sets_[i] = set;
  }
}

void Maze::AddVerticalWalls(uint32_t row) {
  for (uint32_t i = 0; i < cols_ - 1; ++i) {
    bool random_decision = GenerateRandomNumber();
    if ((sets_[i] == sets_[i + 1]) || random_decision)
      vertical_walls_[row][i] = true;
    else
      MergeSets(i, sets_[i]);
  }
  vertical_walls_[row][cols_ - 1] = true;
}

void Maze::AddHorizontalWalls(uint32_t row) {
  for (uint32_t i = 0; i != cols_; ++i) {
    bool random_decision = GenerateRandomNumber();
    if (CountSetMembers(sets_[i]) > 1 && random_decision)
      horizontal_walls_[row][i] = true;
  }
}

void Maze::CheckForIsolation(uint32_t row) {
  for (uint32_t i = 0; i != cols_; ++i) {
    if (CountSetMembers(sets_[i]) == CountWalls(sets_[i], row))
      horizontal_walls_[row][i] = false;
  }
}

bool Maze::GenerateRandomNumber() {
  srand((unsigned)rand_);
  rand_ = rand();
  return rand_ % 3;
}

uint32_t Maze::CountSetMembers(uint32_t set) {
  uint32_t res = 0;
  for (uint32_t i = 0; i != cols_; ++i) {
    if (sets_[i] == set) ++res;
  }
  return res;
}

uint32_t Maze::CountWalls(uint32_t set, uint32_t row) {
  uint32_t res = 0;
  for (uint32_t i = 0; i != cols_; ++i) {
    if ((sets_[i] == set) && (horizontal_walls_[row][i])) ++res;
  }
  return res;
}

void Maze::PrepareSetsForNewLine(uint32_t row) {
  for (uint32_t i = 0; i != cols_; ++i) {
    if (horizontal_walls_[row][i]) sets_[i] = 0;
  }
}

void Maze::GenerateLastLine() {
  JoinToUniqueSet();
  AddVerticalWalls(rows_ - 1);
  CheckLastLine();
}

void Maze::CheckLastLine() {
  for (uint32_t i = 0; i != cols_ - 1; ++i) {
    if (sets_[i] != sets_[i + 1]) {
      vertical_walls_[rows_ - 1][i] = false;
      MergeSets(i, sets_[i]);
    }
    horizontal_walls_[rows_ - 1][i] = true;
  }
  vertical_walls_[rows_ - 1][cols_ - 1] = true;
  horizontal_walls_[rows_ - 1][cols_ - 1] = true;
}

void Maze::SaveToFile(std::string filename) {
  std::ofstream output;
  output.open(filename);
  if (output.is_open()) {
    output << rows_ << " " << cols_ << std::endl;
    for (uint32_t i = 0; i < rows_; ++i) {
      for (uint32_t y = 0; y < cols_ - 1; ++y) {
        output << vertical_walls_[i][y] << " ";
      }
      output << vertical_walls_[i][cols_ - 1] << std::endl;
    }
    output << std::endl;
    for (uint32_t i = 0; i < rows_; ++i) {
      for (uint32_t y = 0; y < cols_ - 1; ++y) {
        output << horizontal_walls_[i][y] << " ";
      }
      output << horizontal_walls_[i][cols_ - 1] << std::endl;
    }
    output.close();
  } else
    throw std::logic_error("Couldn't open file.");
}

void Maze::InitStructures() {
  CreateMatrix(&vertical_walls_, false);
  CreateMatrix(&horizontal_walls_, false);
  PrepareFirstLine();
}

void Maze::FreeStructures() {
  if (MatrixNotEmpty(&vertical_walls_)) DeleteMatrix(&vertical_walls_);
  if (MatrixNotEmpty(&horizontal_walls_)) DeleteMatrix(&horizontal_walls_);
  if (MatrixNotEmpty(&maze_cells_)) DeleteMatrix(&maze_cells_);
  if (MatrixNotEmpty(&answer_matrix_)) DeleteMatrix(&answer_matrix_);
  if (answer_ != nullptr) delete answer_;
  rows_ = 0;
  cols_ = 0;
}

uint32_t Maze::Solve(Point start, Point finish) {
  uint32_t status = 1, passable_cells = 0;
  if (!MatrixNotEmpty(&vertical_walls_) ||
      !(MatrixNotEmpty(&horizontal_walls_)))
    throw std::logic_error("Can't find a solution for a non-existent maze");
  if (start.row >= rows_ || start.col >= cols_ || finish.row >= rows_ ||
      finish.col >= cols_)
    throw std::out_of_range("Wrong coordinates");
  CreateMatrix(&maze_cells_, -1);
  int32_t counter = 0, exit_found = 0;
  maze_cells_[start.row][start.col] = counter;
  Point current_cell = start;
  while ((exit_found == 0) && (maze_cells_[finish.row][finish.col] == -1)) {
    for (uint32_t i = 0; i != rows_; ++i) {
      for (uint32_t y = 0; y != cols_; ++y) {
        if (maze_cells_[i][y] == counter) {
          current_cell.row = i;
          current_cell.col = y;
          if (!WaveCanSpread(current_cell))
            passable_cells -= 1;
          else
            passable_cells += MarkCells(current_cell, counter);
          if (!passable_cells) exit_found = -1;
        }
      }
    }
    ++counter;
  }
  if (maze_cells_[finish.row][finish.col] != -1) exit_found = 1;
  if (exit_found == 1) answer_ = TraceWay(start, finish);
  if (exit_found == -1) {
    delete answer_;
    answer_ = new solution;
    status = 0;
  }
  return status;
}

bool Maze::WaveCanSpread(Point current) {
  bool res = false;
  if ((WaveCanGoDown(current)) || (WaveCanGoUp(current)) ||
      (WaveCanGoLeft(current)) || (WaveCanGoRight(current)))
    res = true;
  return res;
}

uint32_t Maze::MarkCells(Point current, uint32_t counter) {
  uint32_t status = 0;
  uint32_t new_value = counter + 1;
  if (WaveCanGoUp(current)) {
    maze_cells_[current.row - 1][current.col] = new_value;
    ++status;
  }
  if (WaveCanGoLeft(current)) {
    maze_cells_[current.row][current.col - 1] = new_value;
    ++status;
  }
  if (WaveCanGoDown(current)) {
    maze_cells_[current.row + 1][current.col] = new_value;
    ++status;
  }
  if (WaveCanGoRight(current)) {
    maze_cells_[current.row][current.col + 1] = new_value;
    ++status;
  }
  return status;
}

bool Maze::WaveCanGoUp(Point current) {
  bool res = false;
  if ((current.row > 0) && (maze_cells_[current.row - 1][current.col] == -1) &&
      !horizontal_walls_[current.row - 1][current.col])
    res = true;
  return res;
}

bool Maze::WaveCanGoDown(Point current) {
  bool res = false;
  if (((current.row + 1) < rows_) &&
      (maze_cells_[current.row + 1][current.col] == -1) &&
      !horizontal_walls_[current.row][current.col])
    res = true;
  return res;
}

bool Maze::WaveCanGoLeft(Point current) {
  bool res = false;
  if ((current.col > 0) && (maze_cells_[current.row][current.col - 1] == -1) &&
      !vertical_walls_[current.row][current.col - 1])
    res = true;
  return res;
}

bool Maze::WaveCanGoRight(Point current) {
  bool res = false;
  if (((current.col + 1) < cols_) &&
      (maze_cells_[current.row][current.col + 1] == -1) &&
      !vertical_walls_[current.row][current.col])
    res = true;
  return res;
}

std::stack<std::pair<uint32_t, uint32_t>> *Maze::TraceWay(Point start,
                                                          Point finish) {
  solution *answer = new solution;
  answer->emplace(finish.row, finish.col);
  if ((start.row != finish.row) || (start.col != finish.col)) {
    Point current = finish;
    do {
      for (uint32_t i = 0; i != rows_; ++i) {
        for (uint32_t y = 0; y != cols_; ++y) {
          current = FindNextCell(current);
          if ((i == 0 && y == 0) || ((answer->top().first != current.row) ||
                                     (answer->top().second != current.col)))
            answer->emplace(current.row, current.col);
        }
      }
    } while ((current.row != start.row) || (current.col != start.col));
  }
  return answer;
}

Point Maze::FindNextCell(Point current) {
  Point result = current;
  if ((current.row > 0) &&
      (maze_cells_[current.row - 1][current.col] ==
       (maze_cells_[current.row][current.col] - 1)) &&
      (!horizontal_walls_[current.row - 1][current.col])) {
    result.row = current.row - 1;
  } else if ((current.col > 0) &&
             (maze_cells_[current.row][current.col - 1] ==
              (maze_cells_[current.row][current.col] - 1)) &&
             (!vertical_walls_[current.row][current.col - 1])) {
    result.col = current.col - 1;
  } else if (((current.row + 1) < rows_) &&
             (maze_cells_[current.row + 1][current.col] ==
              (maze_cells_[current.row][current.col] - 1)) &&
             (!horizontal_walls_[current.row][current.col])) {
    result.row = current.row + 1;
  } else if (((current.col + 1) < cols_) &&
             (maze_cells_[current.row][current.col + 1] ==
              (maze_cells_[current.row][current.col] - 1)) &&
             (!vertical_walls_[current.row][current.col])) {
    result.col = current.col + 1;
  }
  return result;
}

}  // namespace s21
