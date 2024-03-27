#include "../model/cave.h"
#include "gtest/gtest.h"

TEST(TestCave, Test_1) {
  s21::Cave cave;
  cave.Init(50, 50, 0.5);
  EXPECT_EQ(cave.getWidth(), (size_t)50);
  EXPECT_EQ(cave.getHeight(), (size_t)50);
}

TEST(TestCave, Test_2) {
  s21::Cave cave;
  EXPECT_THROW(cave.Init(0, 50, 0.5), std::logic_error);
  EXPECT_THROW(cave.Init(1, 51, 0.5), std::logic_error);
  EXPECT_THROW(cave.Init(-1, -1, 0.5), std::logic_error);
  EXPECT_THROW(cave.Init(60, 5, 0.5), std::logic_error);
}

TEST(TestCave, Test_3) {
  s21::Cave cave;
  cave.LoadFromFile("examples/cave01.txt");
  EXPECT_EQ(cave.getWidth(), (size_t)4);
  EXPECT_EQ(cave.getHeight(), (size_t)4);
}

TEST(TestCave, Test_4) {
  s21::Cave cave;
  EXPECT_THROW(cave.LoadFromFile("examples/cave_err1.txt"), std::logic_error);
  EXPECT_THROW(cave.LoadFromFile("examples/cave_err2.txt"), std::logic_error);
  EXPECT_THROW(cave.LoadFromFile("examples/cave_err3.txt"), std::logic_error);
}

TEST(TestCave, Test_5) {
  s21::Cave cave;
  cave.LoadFromFile("examples/cave01.txt");
  cave.NextStep(7, 7);
  std::vector<std::vector<bool>> matrix = cave.getCave();
  size_t width = cave.getWidth();
  size_t height = cave.getHeight();
  for (size_t i = 1; i <= height; ++i) {
    for (size_t j = 1; j <= width; ++j) {
      EXPECT_EQ(matrix[i][j], false);
    }
  }
}

TEST(TestCave, Test_6) {
  s21::Cave cave;
  cave.LoadFromFile("examples/cave01.txt");
  cave.NextStep(1, 1);
  std::vector<std::vector<bool>> matrix = cave.getCave();
  size_t width = cave.getWidth();
  size_t height = cave.getHeight();
  for (size_t i = 1; i <= height; ++i) {
    for (size_t j = 1; j <= width; ++j) {
      EXPECT_EQ(matrix[i][j], true);
    }
  }
}