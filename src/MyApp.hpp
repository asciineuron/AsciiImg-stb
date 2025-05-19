#ifndef MYAPP_H
#define MYAPP_H

#include "mylib.hpp"
#include <QWidget>

class QPushButton;
class QSlider;
class QLabel;
class QImage;
class QPixmap;
class QGraphicsScene;
class QGraphicsView;
class QGraphicsPixmapItem;
class QLineEdit;

class MyApp : public QWidget {
  Q_OBJECT

public:
  explicit MyApp(QWidget* parent = nullptr);
  ~MyApp();

private:
  QPushButton* m_runButton;
  QPushButton* m_loadButton;
  QSlider* m_fontSizeSlider;
  QLineEdit* m_lineEdit;
  QGraphicsScene* m_graphicsScene;
  QGraphicsView* m_graphicsView;
  int m_sliderValue = 10;

  std::unique_ptr<MyLib::AsciiImg> m_asciiImg;

private slots:
  void onButtonClicked();
  void onLoadButtonClicked();
  void onSliderValueReleased();
};

#endif
