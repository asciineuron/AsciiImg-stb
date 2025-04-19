#ifndef MYAPP_H

#include <QWidget>

class QPushButton;

class MyApp : public QWidget {
  Q_OBJECT

public:
  explicit MyApp(QWidget* parent = nullptr);
  ~MyApp();
private:
  QPushButton* RunButton;
private slots:
  void onButtonReleased();
};

#endif
