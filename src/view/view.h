#ifndef A1_MAZE_SRC_VIEW_VIEW_H_
#define A1_MAZE_SRC_VIEW_VIEW_H_

#include <QMainWindow>

#include "../model/cave.h"
#include "../model/maze.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class View;
}
QT_END_NAMESPACE

class View : public QMainWindow {
  Q_OBJECT

 public:
  View(QWidget *parent = nullptr);
  ~View();

 protected:
  void paintEvent(QPaintEvent *event);

  Ui::View *ui;
  s21::Cave cave_;
  s21::Maze *maze_ = nullptr;
  QString path_;
  QTimer *timer_ = nullptr;

 private slots:
  void on_pushButton_5_clicked();
  void on_pushButton_6_clicked();
  void on_pushButton_4_clicked();
  void on_pushButton_7_clicked();
  void on_pushButton_clicked();
  void on_pushButton_3_clicked();
  void on_pushButton_2_clicked();
  void on_spinBox_valueChanged(int arg1);
  void on_spinBox_2_valueChanged(int arg1);

 private:
  const int kDrawZoneLeft = 210;
  const int kDrawZoneTop = 20;
  const int kDrawZoneWidth = 500;
  const int kDrawZoneHeight = 500;

  void DrawMaze(QPainter *painter);
  void DrawCave(QPainter *painter);
};

#endif  // A1_MAZE_SRC_VIEW_VIEW_H_
