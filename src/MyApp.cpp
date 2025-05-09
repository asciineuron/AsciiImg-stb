#include "MyApp.hpp"
#include <QtWidgets>
#include "mylib.hpp"
#include "stb_image.h"

// TODO note actually ok to translate QImage > QPixmap
// imagine: everything is in QImage. Only translate to display
// once per click cycle, but never read qimage from that pixmap

MyApp::MyApp(QWidget* parent) :
  QWidget(parent) {
  m_runButton = new QPushButton(tr("run"));
  //m_image = new QImage("/Volumes/Ext/Code/AsciiImg-stb/scuba.jpg");
  //m_imagemap = new QPixmap;
  //m_imageLabel = new QLabel(tr("hi"));
  //m_imageLabel->setPixmap(*m_imagemap);
  m_graphicsScene = new QGraphicsScene;
  //m_pixmapgraphicsitem = m_graphicsScene->addPixmap(*m_imagemap);
  m_graphicsView = new QGraphicsView(m_graphicsScene);


  QGridLayout* mainLayout = new QGridLayout;
  mainLayout->addWidget(m_graphicsView, 0, 0);
  mainLayout->addWidget(m_runButton, 1, 0);
  setLayout(mainLayout);
  setWindowTitle(tr("myapp"));
 
  // get image displayed
  //m_graphicsScene->addPixmap(QPixmap::fromImage(*m_image));
  //m_graphicsView->fitInView(m_graphicsScene->sceneRect(), Qt::KeepAspectRatio);

  // connect the signals and slots of the objects under my responsibility - makes sense!
  connect(m_runButton, SIGNAL(clicked()), this, SLOT(onButtonClicked()));

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

void MyApp::onButtonClicked() {
  printf("hahahahahaha\n"); 
  //m_imagemap->convertFromImage(*m_image);
  //m_pixmapgraphicsitem->setPixmap(*m_imagemap);
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
