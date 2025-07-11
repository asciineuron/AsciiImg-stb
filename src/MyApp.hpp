#pragma once

#include "mylib.hpp"
#include <QWidget>
#include <QGraphicsView>

class QPushButton;
class QSlider;
class QLabel;
class QImage;
class QPixmap;
class QGraphicsScene;
class QGraphicsView;
class QGraphicsPixmapItem;
class QLineEdit;
class QMimeData;

class MyApp : public QWidget {
  Q_OBJECT

public:
  explicit MyApp(QWidget* parent = nullptr);
  ~MyApp();
  void dragEnterEvent(QDragEnterEvent *event) override;
  void dropEvent(QDropEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;

private:
  QPushButton* m_runButton;
  QPushButton* m_loadButton;
  QPushButton* m_saveButton;
  QSlider* m_fontSizeSlider;
  QSlider* m_padSizeSlider;
  QLineEdit* m_lineEdit;
  QGraphicsScene* m_graphicsScene;
  QGraphicsView* m_graphicsView;
  int m_sliderValue = 10;
  int m_padSliderValue = 0;
  QString m_imagepath {};

  std::unique_ptr<MyLib::AsciiImg> m_asciiImg;

private slots:
  void onButtonClicked();
  void onLoadButtonClicked();
  void onSaveButtonClicked();
  void onSliderValueReleased();
  void onPadSliderValueReleased();
};
