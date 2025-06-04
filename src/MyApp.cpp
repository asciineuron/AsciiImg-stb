#include "MyApp.hpp"
#include "mylib.hpp"
#include "stb_image.h"
#include <iostream>
#include <Qt>
#include <QtWidgets>
#include <QFileDialog>

MyApp::MyApp(QWidget *parent) : QWidget(parent) {
  setAcceptDrops(true);
  m_runButton = new QPushButton(tr("run"));
  m_loadButton = new QPushButton(tr("load image"));
  m_saveButton = new QPushButton(tr("save image"));
  m_saveButton->setEnabled(false);

  m_fontSizeSlider = new QSlider(Qt::Orientation::Horizontal);
  m_lineEdit = new QLineEdit(tr("/Volumes/Ext/Code/AsciiImg-stb/scuba1.jpg"));
  m_graphicsScene = new QGraphicsScene;
  m_graphicsView = new QGraphicsView(m_graphicsScene);
  m_graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  m_graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  QGridLayout *mainLayout = new QGridLayout;
  mainLayout->addWidget(m_graphicsView, 0, 0);
  mainLayout->addWidget(m_saveButton, 0, 1);
  mainLayout->addWidget(m_lineEdit, 1, 0);
  mainLayout->addWidget(m_loadButton, 1, 1);
  mainLayout->addWidget(m_fontSizeSlider, 2, 0);
  mainLayout->addWidget(m_runButton, 2, 1);
  setLayout(mainLayout);
  setWindowTitle(tr("Ascii-Img"));

  connect(m_runButton, &QAbstractButton::clicked, this,
          &MyApp::onButtonClicked);
  connect(m_fontSizeSlider, &QSlider::sliderReleased, this,
          &MyApp::onSliderValueReleased);
  connect(m_loadButton, &QAbstractButton::clicked, this,
          &MyApp::onLoadButtonClicked);
  connect(m_saveButton, &QAbstractButton::clicked, this,
          &MyApp::onSaveButtonClicked);

  m_asciiImg = std::make_unique<MyLib::AsciiImg>(
      10, "/Volumes/Ext/Code/AsciiImg-stb/DejaVuSansMono.ttf",
      "/Volumes/Ext/Code/AsciiImg-stb/scuba1.jpg");

  m_graphicsScene->addPixmap(QPixmap::fromImage(m_asciiImg->getOrigImage()));
  m_graphicsView->fitInView(m_graphicsScene->sceneRect(), Qt::KeepAspectRatio);
}

MyApp::~MyApp() {
  // all widgets managed by gridlayout and deleted there
}

void MyApp::resizeEvent(QResizeEvent *event) {
  m_graphicsView->fitInView(m_graphicsScene->sceneRect(), Qt::KeepAspectRatio);
}

void MyApp::dragEnterEvent(QDragEnterEvent *event) {
  if (event->mimeData()->hasUrls()) {
    event->acceptProposedAction();
  }
}

void MyApp::dropEvent(QDropEvent *event) {
  auto urls = event->mimeData()->urls();
  auto u = urls.begin();
  QString path = u->path(); 
  std::cout << path.toStdString();
  if (m_imagepath != path) {
    m_imagepath = path;
  }

  m_asciiImg->loadImage(m_imagepath.toStdString());
  m_graphicsScene->clear();
  m_graphicsScene->addPixmap(QPixmap::fromImage(m_asciiImg->getOrigImage()));
  m_graphicsView->fitInView(m_graphicsScene->sceneRect(), Qt::KeepAspectRatio);
}

void MyApp::onSliderValueReleased() {
  int currentSliderVal = m_fontSizeSlider->value();
  if (currentSliderVal == m_sliderValue) {
    return; // if slider unchanged do nothing
  }
  m_sliderValue = currentSliderVal;
}

void MyApp::onButtonClicked() {
  m_asciiImg->setTextRows(m_sliderValue + 1);
  m_asciiImg->asciifyImage();
  const QImage& asciifiedImage = m_asciiImg->getAsciifyImage();
  
  m_graphicsScene->clear();
  m_graphicsScene->addPixmap(QPixmap::fromImage(asciifiedImage));
  m_graphicsView->fitInView(m_graphicsScene->sceneRect(), Qt::KeepAspectRatio);

  m_saveButton->setEnabled(true);
}

void MyApp::onLoadButtonClicked() {
  bool res = m_asciiImg->loadImage(m_lineEdit->text().trimmed().toStdString());
  if (!res) {
    m_lineEdit->setText(tr("Please enter a valid image path."));
  }
  // show input image pre-asciified
  m_graphicsScene->clear();
  m_graphicsScene->addPixmap(QPixmap::fromImage(m_asciiImg->getOrigImage()));
  m_graphicsView->fitInView(m_graphicsScene->sceneRect(), Qt::KeepAspectRatio);

  m_saveButton->setEnabled(false);
}

void MyApp::onSaveButtonClicked() {
  QString fileSaveName = QFileDialog::getSaveFileName(this, tr("Save File"), "untitled.png");
  const QImage& asciifiedImage = m_asciiImg->getAsciifyImage(); 
  asciifiedImage.save(fileSaveName);
  printf(fileSaveName.toStdString().c_str());
}
