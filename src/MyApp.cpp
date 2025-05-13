#include "MyApp.hpp"
#include <QtWidgets>
#include "mylib.hpp"
#include "stb_image.h"
#include <Qt>

// TODO note actually ok to translate QImage > QPixmap
// imagine: everything is in QImage. Only translate to display
// once per click cycle, but never read qimage from that pixmap

MyApp::MyApp(QWidget* parent) :
  QWidget(parent) {
  m_runButton = new QPushButton(tr("run"));
  m_fontSizeSlider = new QSlider(Qt::Orientation::Horizontal);
  //m_image = new QImage("/Volumes/Ext/Code/AsciiImg-stb/scuba.jpg");
  m_graphicsScene = new QGraphicsScene;
  m_graphicsView = new QGraphicsView(m_graphicsScene);


  QGridLayout* mainLayout = new QGridLayout;
  mainLayout->addWidget(m_graphicsView, 0, 0);
  mainLayout->addWidget(m_runButton, 1, 0);
  mainLayout->addWidget(m_fontSizeSlider, 2, 0);
  setLayout(mainLayout);
  setWindowTitle(tr("myapp"));
 
  // get image displayed
  //m_graphicsScene->addPixmap(QPixmap::fromImage(*m_image));
  //m_graphicsView->fitInView(m_graphicsScene->sceneRect(), Qt::KeepAspectRatio);

  // connect the signals and slots of the objects under my responsibility - makes sense!
  connect(m_runButton, &QAbstractButton::clicked, this, &MyApp::onButtonClicked);
  //connect(m_fontSizeSlider, SIGNAL(valueChanged()), this, SLOT(onSliderValueChanged());
  connect(m_fontSizeSlider, &QSlider::sliderReleased, this, &MyApp::onSliderValueChanged);

  int ii_n;
  m_imData = stbi_load("/Volumes/Ext/Code/AsciiImg-stb/scuba.jpg", &m_imX, &m_imY, &ii_n, 1);
}

MyApp::~MyApp() {
 //delete m_runButton; if added to layout, layout owns it and will delete, so not needed here
 // and don't need to delete layout since it's managed?
 //delete m_imagemap;
 delete m_image;
 delete m_graphicsScene;
 //delete m_graphicsView;
 //delete m_imageLabel;
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
  //m_imagemap->convertFromImage(*m_image);
  //unsigned char* im_data = m_image->bits();
  int w = 0;
  int h = 0;
  unsigned char* outimage = MyLib::dofunc(m_imData, &w, &h, m_imX, m_imY);
  //m_imageEdited = QImage();
  QImage image_edited(outimage, w, h, QImage::Format_Mono); 

  m_graphicsScene->addPixmap(QPixmap::fromImage(image_edited));
  m_graphicsView->fitInView(m_graphicsScene->sceneRect(), Qt::KeepAspectRatio);
  //m_imageLabel->setPixmap(*m_imagemap);

}
