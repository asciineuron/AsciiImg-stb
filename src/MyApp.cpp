#include "MyApp.hpp"
#include <QtWidgets>

MyApp::MyApp(QWidget* parent) :
  QWidget(parent) {
  m_runButton = new QPushButton(tr("run"));
  m_image = new QImage("/Volumes/Ext/Code/AsciiImg-stb/scuba.jpg");
  m_imagemap = new QPixmap;
  //m_imageLabel = new QLabel(tr("hi"));
  //m_imageLabel->setPixmap(*m_imagemap);
  m_graphicsScene = new QGraphicsScene;
  m_pixmapgraphicsitem = m_graphicsScene->addPixmap(*m_imagemap);
  m_graphicsView = new QGraphicsView(m_graphicsScene);


  QGridLayout* mainLayout = new QGridLayout;
  mainLayout->addWidget(m_graphicsView, 0, 0);
  mainLayout->addWidget(m_runButton, 1, 0);
  setLayout(mainLayout);
  setWindowTitle(tr("myapp"));

  // connect the signals and slots of the objects under my responsibility - makes sense!
  connect(m_runButton, SIGNAL(clicked()), this, SLOT(onButtonClicked()));
}

MyApp::~MyApp() {
 delete m_runButton; 
 delete m_imagemap;
 delete m_image;
 delete m_graphicsScene;
 delete m_graphicsView;
 //delete m_imageLabel;
}

void MyApp::onButtonClicked() {
  printf("hahahahahaha\n"); 
  m_imagemap->convertFromImage(*m_image);
  m_pixmapgraphicsitem->setPixmap(*m_imagemap);
  //m_imageLabel->setPixmap(*m_imagemap);

}
