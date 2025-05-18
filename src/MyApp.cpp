#include "MyApp.hpp"
#include "mylib.hpp"
#include "stb_image.h"
#include <Qt>
#include <QtWidgets>

MyApp::MyApp(QWidget *parent) : QWidget(parent) {
  m_runButton = new QPushButton(tr("run"));
  m_loadButton = new QPushButton(tr("load image"));
  m_fontSizeSlider = new QSlider(Qt::Orientation::Horizontal);
  m_lineEdit = new QLineEdit(tr("enter path to image"));
  m_graphicsScene = new QGraphicsScene;
  m_graphicsView = new QGraphicsView(m_graphicsScene);

  QGridLayout *mainLayout = new QGridLayout;
  mainLayout->addWidget(m_graphicsView, 0, 0);
  mainLayout->addWidget(m_lineEdit, 1, 0);
  mainLayout->addWidget(m_loadButton, 1, 1);
  mainLayout->addWidget(m_fontSizeSlider, 2, 0);
  mainLayout->addWidget(m_runButton, 2, 1);
  setLayout(mainLayout);
  setWindowTitle(tr("myapp"));

  connect(m_runButton, &QAbstractButton::clicked, this,
          &MyApp::onButtonClicked);
  connect(m_fontSizeSlider, &QSlider::sliderReleased, this,
          &MyApp::onSliderValueChanged);
  connect(m_loadButton, &QAbstractButton::clicked, this,
          &MyApp::onLoadButtonClicked);

  m_asciiImg = std::make_unique<MyLib::AsciiImg>(
      10, "/Volumes/Ext/Code/AsciiImg-stb/DejaVuSansMono.ttf",
      "/Volumes/Ext/Code/AsciiImg-stb/scuba1.jpg");
}

MyApp::~MyApp() {
  // all widgets managed by gridlayout and deleted there
}

void MyApp::onSliderValueChanged() {
  int currentSliderVal = m_fontSizeSlider->value();
  if (currentSliderVal == m_sliderValue) {
    return; // if slider unchanged do nothing
  }
  m_sliderValue = currentSliderVal;
  printf("slideee %d\n", m_sliderValue);
}

void MyApp::onButtonClicked() {
  m_asciiImg->setTextRows(m_sliderValue + 1);
  QImage asciifiedImage = m_asciiImg->asciifyImage();
  
  m_graphicsScene->clear();
  m_graphicsScene->addPixmap(QPixmap::fromImage(asciifiedImage));
  m_graphicsView->fitInView(m_graphicsScene->sceneRect(), Qt::KeepAspectRatio);
}

void MyApp::onLoadButtonClicked() {
  bool res = m_asciiImg->loadImage(m_lineEdit->text().trimmed().toStdString());
  if (!res) {
    m_lineEdit->setText(tr("Please enter a valid image path."));
  }
}
