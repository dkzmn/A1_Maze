/*!
 * @mainpage
 * @author  scrimgew, keenanbu
 * @brief This is a C++ program with a Qt-based interface that can generate and
 * render perfect mazes and caves. It can also solve the mazes (if a solution
 * exists).
 * @version 1.0
 * @date 2024-02-02
 * @warning This program is an educational project for School 21
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <QApplication>

#include "view/view.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  View w;
  w.show();
  return a.exec();
}
