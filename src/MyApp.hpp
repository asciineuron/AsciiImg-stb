#ifndef MYAPP_H

#include <QWidget>

class QPushButton;
class QLabel;
class QImage;
class QPixmap;
class QGraphicsScene;
class QGraphicsView;
class QGraphicsPixmapItem;

class MyApp : public QWidget {
  Q_OBJECT

public:
  explicit MyApp(QWidget* parent = nullptr);
  ~MyApp();
private:
  QPushButton* m_runButton;
  //QLabel* m_imageLabel;
  QGraphicsScene* m_graphicsScene;
  QGraphicsView* m_graphicsView;
  QImage* m_image;
  QPixmap* m_imagemap;
  //QGraphicsPixmapItem* m_pixmapgraphicsitem;

private slots:
  void onButtonClicked();
};

#endif
