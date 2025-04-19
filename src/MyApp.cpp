#include "MyApp.hpp"
#include <QtWidgets>

MyApp::MyApp(QWidget* parent) :
  QWidget(parent) {
  RunButton = new QPushButton(tr("run"));

  QGridLayout* MainLayout = new QGridLayout;
  MainLayout->addWidget(RunButton, 0, 0);
  setLayout(MainLayout);
  setWindowTitle(tr("myapp"));

  // connect the signals and slots of the objects under my responsibility - makes sense!
  connect(RunButton, SIGNAL(released()), this, SLOT(onButtonReleased()));
}

MyApp::~MyApp() {
 delete RunButton; 
}

void MyApp::onButtonReleased() {
 printf("hahahahahaha\n"); 
}
