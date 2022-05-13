#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QImage img("/home/efe/Desktop/resimm.png");
    m_ig = new ImageInverter(img, img.height()/2);
    connect(m_ig, &ImageInverter::sigImageReady, this, &MainWindow::slotImage);
    m_ig->start();
}

MainWindow::~MainWindow()
{   m_ig->stop();
    if(m_ig){
        delete m_ig;
        m_ig = nullptr;
    }
    delete ui;
}

void MainWindow::slotImage(QImage *img, QMutex *mutex)
{
    mutex->lock();
    ui->label->setPixmap(QPixmap::fromImage(*img));
    mutex->unlock();
}

