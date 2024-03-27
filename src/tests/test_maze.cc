#include "../model/maze.h"
#include "gtest/gtest.h"

TEST(TestMaze, Test_1) {
  s21::Maze *maze = new s21::Maze();
  EXPECT_EQ(maze->GetRows(), (uint32_t)1);
  EXPECT_EQ(maze->GetCols(), (uint32_t)1);
  delete maze;
}

TEST(TestMaze, Test_2) {
  s21::Maze *maze = new s21::Maze(25, 25);
  EXPECT_EQ(maze->GetRows(), (uint32_t)25);
  EXPECT_EQ(maze->GetCols(), (uint32_t)25);
  delete maze;
}

TEST(TestMaze, Test_3) {
  EXPECT_THROW(new s21::Maze((uint32_t)0.5, (uint32_t)25), std::out_of_range);
  EXPECT_THROW(new s21::Maze((uint32_t)0, (uint32_t)25), std::out_of_range);
  EXPECT_THROW(new s21::Maze((uint32_t)1, (uint32_t)0), std::out_of_range);
  EXPECT_THROW(new s21::Maze((uint32_t)12, (uint32_t)55), std::out_of_range);
}

TEST(TestMaze, Test_4) {
  s21::Maze *maze = new s21::Maze();
  maze->LoadFromFile("examples/maze_01.txt");
  EXPECT_EQ(maze->GetRows(), (uint32_t)10);
  EXPECT_EQ(maze->GetCols(), (uint32_t)10);
  delete maze;
}

TEST(TestMaze, Test_5) {
  s21::Maze *maze = new s21::Maze();
  EXPECT_THROW(maze->LoadFromFile("/examples/maze_01.txt"), std::logic_error);
  delete maze;
}

TEST(TestMaze, Test_6) {
  s21::Maze *maze = new s21::Maze();
  EXPECT_THROW(maze->LoadFromFile("examples/maze_wrong_01.txt"),
               std::length_error);
  EXPECT_THROW(maze->LoadFromFile("examples/maze_wrong_02.txt"),
               std::length_error);
  EXPECT_THROW(maze->LoadFromFile("examples/maze_wrong_03.txt"),
               std::length_error);
  EXPECT_THROW(maze->LoadFromFile("examples/maze_wrong_04.txt"),
               std::out_of_range);
  EXPECT_THROW(maze->LoadFromFile("examples/maze_wrong_05.txt"),
               std::length_error);
  delete maze;
}

TEST(TestMaze, Test_7) {
  s21::Maze *maze = new s21::Maze(20, 20);
  maze->Generate();
  maze->SaveToFile("examples/generated_maze.txt");
  s21::Maze *maze_generated = new s21::Maze();
  maze_generated->LoadFromFile("examples/generated_maze.txt");
  EXPECT_EQ(maze->GetRows(), maze_generated->GetRows());
  EXPECT_EQ(maze->GetCols(), maze_generated->GetCols());
  s21::MazePointers m1 = maze->GetMazeMatrices();
  s21::MazePointers m2 = maze_generated->GetMazeMatrices();
  for (uint32_t i = 0; i != maze->GetRows(); ++i) {
    for (uint32_t y = 0; y != maze->GetCols(); ++y) {
      EXPECT_EQ(m1.vertical_walls[i][y], m2.vertical_walls[i][y]);
      EXPECT_EQ(m1.horizontal_walls[i][y], m2.horizontal_walls[i][y]);
    }
  }
  delete maze;
  delete maze_generated;
}

TEST(TestMaze, Test_8) {
  s21::Maze *maze = new s21::Maze();
  maze->LoadFromFile("examples/maze_01.txt");
  maze->Solve({0, 0}, {9, 9});
  s21::solution result;
  result.emplace(9, 9);
  result.emplace(9, 8);
  result.emplace(9, 7);
  result.emplace(9, 6);
  result.emplace(8, 6);
  result.emplace(7, 6);
  result.emplace(7, 5);
  result.emplace(6, 5);
  result.emplace(6, 6);
  result.emplace(6, 7);
  result.emplace(5, 7);
  result.emplace(5, 8);
  result.emplace(6, 8);
  result.emplace(6, 9);
  result.emplace(5, 9);
  result.emplace(4, 9);
  result.emplace(3, 9);
  result.emplace(2, 9);
  result.emplace(1, 9);
  result.emplace(0, 9);
  result.emplace(0, 8);
  result.emplace(1, 8);
  result.emplace(2, 8);
  result.emplace(3, 8);
  result.emplace(3, 7);
  result.emplace(2, 7);
  result.emplace(1, 7);
  result.emplace(1, 6);
  result.emplace(1, 5);
  result.emplace(1, 4);
  result.emplace(0, 4);
  result.emplace(0, 3);
  result.emplace(1, 3);
  result.emplace(2, 3);
  result.emplace(2, 4);
  result.emplace(2, 5);
  result.emplace(3, 5);
  result.emplace(3, 4);
  result.emplace(4, 4);
  result.emplace(4, 3);
  result.emplace(3, 3);
  result.emplace(3, 2);
  result.emplace(3, 1);
  result.emplace(2, 1);
  result.emplace(2, 2);
  result.emplace(1, 2);
  result.emplace(0, 2);
  result.emplace(0, 1);
  result.emplace(0, 0);
  s21::solution m1 = maze->GetSolution();
  while (!result.empty()) {
    EXPECT_EQ(result.top().first, m1.top().first);
    EXPECT_EQ(result.top().second, m1.top().second);
    result.pop();
    m1.pop();
  }
  delete maze;
}

TEST(TestMaze, Test_9) {
  s21::Maze *maze = new s21::Maze(20, 20);
  maze->Generate();
  EXPECT_THROW(maze->SaveToFile("/examples/generated_maze.txt"),
               std::logic_error);
  delete maze;
}

TEST(TestMaze, Test_10) {
  s21::Maze *maze = new s21::Maze();
  EXPECT_THROW(maze->Solve({0, 0}, {5, 5}), std::logic_error);
  delete maze;
}

TEST(TestMaze, Test_11) {
  s21::Maze *maze = new s21::Maze();
  maze->LoadFromFile("examples/maze_02.txt");
  maze->Solve({9, 9}, {0, 0});
  s21::solution result;
  result.emplace(0, 0);
  result.emplace(1, 0);
  result.emplace(2, 0);
  result.emplace(3, 0);
  result.emplace(4, 0);
  result.emplace(4, 1);
  result.emplace(3, 1);
  result.emplace(2, 1);
  result.emplace(2, 2);
  result.emplace(2, 3);
  result.emplace(1, 3);
  result.emplace(1, 2);
  result.emplace(0, 2);
  result.emplace(0, 3);
  result.emplace(0, 4);
  result.emplace(0, 5);
  result.emplace(0, 6);
  result.emplace(1, 6);
  result.emplace(2, 6);
  result.emplace(3, 6);
  result.emplace(4, 6);
  result.emplace(4, 5);
  result.emplace(4, 4);
  result.emplace(5, 4);
  result.emplace(5, 3);
  result.emplace(6, 3);
  result.emplace(7, 3);
  result.emplace(7, 4);
  result.emplace(7, 5);
  result.emplace(8, 5);
  result.emplace(8, 6);
  result.emplace(8, 7);
  result.emplace(7, 7);
  result.emplace(6, 7);
  result.emplace(6, 6);
  result.emplace(6, 5);
  result.emplace(5, 5);
  result.emplace(5, 6);
  result.emplace(5, 7);
  result.emplace(4, 7);
  result.emplace(4, 8);
  result.emplace(3, 8);
  result.emplace(3, 9);
  result.emplace(4, 9);
  result.emplace(5, 9);
  result.emplace(6, 9);
  result.emplace(6, 8);
  result.emplace(7, 8);
  result.emplace(8, 8);
  result.emplace(8, 9);
  result.emplace(9, 9);
  s21::solution m1 = maze->GetSolution();
  while (!result.empty()) {
    EXPECT_EQ(result.top().first, m1.top().first);
    EXPECT_EQ(result.top().second, m1.top().second);
    result.pop();
    m1.pop();
  }
  delete maze;
}

TEST(TestMaze, Test_12) {
  s21::Maze *maze = new s21::Maze();
  maze->LoadFromFile("examples/maze_without_path.txt");
  uint32_t result = maze->Solve({4, 4}, {0, 0});
  EXPECT_EQ(result, (uint32_t)0);
  delete maze;
}

TEST(TestMaze, Test_13) {
  s21::Maze *maze = new s21::Maze();
  maze->LoadFromFile("examples/maze_01.txt");
  EXPECT_THROW(maze->Solve({4, 4}, {0, 11}), std::out_of_range);
  delete maze;
}
