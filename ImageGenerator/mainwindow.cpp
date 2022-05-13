#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsScene>
#include <QPixmap>
#include <QGraphicsView>
#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QImage>
#include <QObject>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_ig = new ImageGenerator(30, 30);
    connect(m_ig, &ImageGenerator::sigImageReady, this, &MainWindow::slotImage);
    m_ig ->start();
}

MainWindow::~MainWindow()
{
    m_ig->stop();
    if(m_ig)
    {
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


