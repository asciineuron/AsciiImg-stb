#ifndef MYAPP_H
#define MYAPP_H

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

class MyDragView : public QGraphicsView {
  Q_OBJECT

public:
  explicit MyDragView(QWidget* parent = nullptr);
  explicit MyDragView(QGraphicsScene *scene, QWidget *parent = nullptr);

signals:
  void changed(const QMimeData *mimeData = nullptr);

private:

protected:
  //void dragEnterEvent(QDragEnterEvent *event) override;
  //void dragMoveEvent(QDragMoveEvent *event) override;
  //void dragLeaveEvent(QDragLeaveEvent *event) override;
  void dropEvent(QDropEvent *event) override;
};

class MyApp : public QWidget {
  Q_OBJECT

public:
  explicit MyApp(QWidget* parent = nullptr);
  ~MyApp();

private:
  QPushButton* m_runButton;
  QPushButton* m_loadButton;
  QPushButton* m_saveButton;
  QSlider* m_fontSizeSlider;
  QLineEdit* m_lineEdit;
  QGraphicsScene* m_graphicsScene;
  //QGraphicsView* m_graphicsView;
  MyDragView* m_graphicsView;
  int m_sliderValue = 10;

  std::unique_ptr<MyLib::AsciiImg> m_asciiImg;

private slots:
  void onButtonClicked();
  void onLoadButtonClicked();
  void onSaveButtonClicked();
  void onSliderValueReleased();
  void onDropChanged();
};

#endif
