#ifndef A1_MAZE_SRC_MODEL_CAVE_H_
#define A1_MAZE_SRC_MODEL_CAVE_H_

#include <string>
#include <vector>

namespace s21 {

/*!
  \brief Cave class
  Stores the cave itself, allows you to create, initialize and change cave.
*/

class Cave {
 public:
  Cave() = default;
  ~Cave() = default;

  /*!
  Initializes the cave
  \param[in] width (1-50)
  \param[in] height (1-50)
  \param[in] chance Probability that the next cell will be live
  \throw std::logic_error In case of incorrect sizes
  */

  void Init(size_t width, size_t height, double chance);

  /*!
  Initializes the cave with data from a file
  \param[in] filename
  \throw std::logic_error In case of an incorrect file or incorrect sizes
  */

  void LoadFromFile(std::string filename);

  /*!
  Takes one step that changes the state of the cave
  \param[in] birth_limit Limit of living cells around making a dead cell alive
  \param[in] death_limit Limit of living cells around making a living cell dead
  */

  void NextStep(int birth_limit, int death_limit) noexcept;

  /*!
  Returns the state matrix of the cave
  \return Matrix
  */

  std::vector<std::vector<bool>> getCave() const noexcept;

  /*!
  Returns the width of the cave
  \return Width
  */

  size_t getWidth() const noexcept;

  /*!
  Returns the height of the cave
  \return Height
  */

  size_t getHeight() const noexcept;

 private:
  size_t width_ = 0;
  size_t height_ = 0;
  std::vector<std::vector<bool>> matrix_;
};

}  // namespace s21

#endif  // A1_MAZE_SRC_MODEL_CAVE_H_
