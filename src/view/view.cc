#include "view.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QtGui>

#include "./ui_view.h"

View::View(QWidget *parent) : QMainWindow(parent), ui(new Ui::View) {
  ui->setupUi(this);
  path_ = QDir::currentPath();
  if (path_ == "/") path_ = QDir::homePath();
  ui->spinBox_3->setMaximum(ui->spinBox->value());
  ui->spinBox_5->setMaximum(ui->spinBox->value());
  ui->spinBox_4->setMaximum(ui->spinBox_2->value());
  ui->spinBox_6->setMaximum(ui->spinBox_2->value());
}

View::~View() { delete ui; }

void View::on_pushButton_5_clicked() {
  cave_.Init(ui->spinBox_7->value(), ui->spinBox_8->value(),
             ui->doubleSpinBox->value());
}

void View::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  QPainter painter(this);
  QPen pen(Qt::black);
  pen.setWidth(2);
  painter.setPen(pen);
  if (ui->tabWidget->currentIndex() == 0) {
    if (maze_ != nullptr) {
      DrawMaze(&painter);
    }
  } else if (ui->tabWidget->currentIndex() == 1) {
    DrawCave(&painter);
  }
  update();
  painter.end();
}

void View::DrawMaze(QPainter *painter) {
  painter->drawRect(
      QRect(kDrawZoneLeft, kDrawZoneTop, kDrawZoneWidth, kDrawZoneHeight));
  painter->fillRect(kDrawZoneLeft + 1, kDrawZoneTop + 1, kDrawZoneWidth - 1,
                    kDrawZoneHeight - 1, Qt::white);
  int width = maze_->GetCols();
  int height = maze_->GetRows();
  double rect_w = (double)kDrawZoneWidth / width;
  double rect_h = (double)kDrawZoneHeight / height;
  s21::MazePointers maze_results = maze_->GetMazeMatrices();
  if (width > 0 && height > 0 && maze_results.vertical_walls != nullptr &&
      maze_results.horizontal_walls != nullptr) {
    for (uint32_t i = 0; i < height; ++i) {
      for (uint32_t j = 0; j < width; ++j) {
        if (maze_results.horizontal_walls[i][j]) {
          painter->drawLine(kDrawZoneLeft + j * rect_w,
                            kDrawZoneTop + round((i + 1) * rect_h),
                            kDrawZoneLeft + round((j + 1) * rect_w),
                            kDrawZoneTop + round((i + 1) * rect_h));
        }
        if (maze_results.vertical_walls[i][j]) {
          painter->drawLine(kDrawZoneLeft + round((j + 1) * rect_w),
                            kDrawZoneTop + round(i * rect_h),
                            kDrawZoneLeft + round((j + 1) * rect_w),
                            kDrawZoneTop + round((i + 1) * rect_h));
        }
      }
    }
  }
  std::stack<std::pair<uint32_t, uint32_t>> solution = maze_->GetSolution();
  QPen pen(Qt::red);
  pen.setWidth(2);
  painter->setPen(pen);
  painter->setBrush(Qt::red);
  painter->drawEllipse(
      QPoint(kDrawZoneLeft + (ui->spinBox_3->value() - 1) * rect_w + rect_w / 2,
             kDrawZoneTop + (ui->spinBox_4->value() - 1) * rect_h + rect_h / 2),
      4, 4);
  painter->drawEllipse(
      QPoint(kDrawZoneLeft + (ui->spinBox_5->value() - 1) * rect_w + rect_w / 2,
             kDrawZoneTop + (ui->spinBox_6->value() - 1) * rect_h + rect_h / 2),
      4, 4);
  if (solution.size() > 0) {
    size_t size = solution.size();
    while (solution.size() > 0) {
      std::pair<uint32_t, uint32_t> point = solution.top();
      solution.pop();
      if (solution.size() > 0) {
        std::pair<uint32_t, uint32_t> next = solution.top();
        painter->drawLine(kDrawZoneLeft + point.second * rect_w + rect_w / 2,
                          kDrawZoneTop + point.first * rect_h + rect_h / 2,
                          kDrawZoneLeft + next.second * rect_w + rect_w / 2,
                          kDrawZoneTop + next.first * rect_h + rect_h / 2);
      }
    }
  }
}

void View::DrawCave(QPainter *painter) {
  int width = cave_.getWidth();
  int height = cave_.getHeight();
  std::vector<std::vector<bool>> matrix = cave_.getCave();
  if (width > 0 && height > 0) {
    int rect_w = kDrawZoneWidth / width;
    int rect_h = kDrawZoneHeight / height;
    for (size_t i = 1; i <= height; ++i) {
      for (size_t j = 1; j <= width; ++j) {
        if (matrix[i][j]) {
          painter->fillRect(kDrawZoneLeft + (j - 1) * rect_w,
                            kDrawZoneTop + (i - 1) * rect_h, rect_w, rect_h,
                            Qt::black);
        } else {
          painter->fillRect(kDrawZoneLeft + (j - 1) * rect_w,
                            kDrawZoneTop + (i - 1) * rect_h, rect_w, rect_h,
                            Qt::white);
        }
      }
    }
  }
}

void View::on_pushButton_6_clicked() {
  cave_.NextStep(ui->spinBox_9->value(), ui->spinBox_10->value());
}

void View::on_pushButton_4_clicked() {
  QString filename = QFileDialog::getOpenFileName(
      this, "Open cave file", path_, "Text Files(*.txt)", 0,
      QFileDialog::DontUseNativeDialog);
  if (QFileInfo::exists(filename)) {
    QFileInfo file(filename);
    path_ = file.absolutePath();
    try {
      cave_.LoadFromFile(filename.toStdString());
      ui->spinBox_7->setValue(cave_.getWidth());
      ui->spinBox_8->setValue(cave_.getHeight());
    } catch (const std::exception &e) {
      QMessageBox msgbox;
      msgbox.setText(e.what());
      msgbox.exec();
    }
  }
}

void View::on_pushButton_7_clicked() {
  if (timer_ == nullptr) {
    timer_ = new QTimer(this);
    connect(timer_, SIGNAL(timeout()), this, SLOT(on_pushButton_6_clicked()));
    timer_->start(ui->spinBox_11->value());
    ui->pushButton_7->setText("Stop");
  } else {
    timer_->stop();
    delete timer_;
    timer_ = nullptr;
    ui->pushButton_7->setText("Auto");
  }
}

void View::on_pushButton_clicked() {
  delete maze_;
  maze_ = new s21::Maze(ui->spinBox_2->value(), ui->spinBox->value());
  maze_->Generate();
  maze_->SaveToFile(
      (path_ + "/" +
       QDateTime::currentDateTime().toString("dd-MMM-yyyy-hh-mm-ss") + ".txt")
          .toStdString());
}

void View::on_pushButton_3_clicked() {
  s21::Point start = {(uint32_t)ui->spinBox_4->value() - 1,
                      (uint32_t)ui->spinBox_3->value() - 1};
  s21::Point stop = {(uint32_t)ui->spinBox_6->value() - 1,
                     (uint32_t)ui->spinBox_5->value() - 1};
  if (maze_->Solve(start, stop) == 0) {
    QMessageBox msgbox;
    msgbox.setText("There is no available solution");
    msgbox.exec();
  }
}

void View::on_pushButton_2_clicked() {
  QString filename = QFileDialog::getOpenFileName(
      this, "Open maze file", path_, "Text Files(*.txt)", 0,
      QFileDialog::DontUseNativeDialog);
  if (QFileInfo::exists(filename)) {
    QFileInfo file(filename);
    path_ = file.absolutePath();
    try {
      delete maze_;
      maze_ = new s21::Maze();
      maze_->LoadFromFile(filename.toStdString());
      ui->spinBox->setValue(maze_->GetCols());
      ui->spinBox_2->setValue(maze_->GetRows());
    } catch (const std::exception &e) {
      QMessageBox msgbox;
      msgbox.setText(e.what());
      msgbox.exec();
    }
  }
}

void View::on_spinBox_valueChanged(int arg1) {
  ui->spinBox_3->setMaximum(arg1);
  ui->spinBox_5->setMaximum(arg1);
}

void View::on_spinBox_2_valueChanged(int arg1) {
  ui->spinBox_4->setMaximum(arg1);
  ui->spinBox_6->setMaximum(arg1);
}
