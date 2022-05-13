#include "imagegenerator.h"
#include <QThread>
#include <QDebug>
#include <QImage>
#include <QTime>
#include <QPixmap>
#include <QGraphicsScene>
#include <QRandomGenerator>
ImageGenerator::ImageGenerator(int width, int height, QObject *parent)
    : QObject{parent}
{
    m_width = width;
    m_height = height;
    m_index = 0;
    moveToThread(new QThread);
    connect(thread(), &QThread::started, this, &ImageGenerator::threadFunc);
    connect(this, &ImageGenerator::sigThreadFinished, thread(), &QThread::quit, Qt::DirectConnection);
    m_img = QImage(width, height, QImage::Format_RGB888);
    m_img.fill(Qt::black);
}

ImageGenerator::~ImageGenerator()
{
    stop();
}

void ImageGenerator::start()
{
    thread()->start();
}

void ImageGenerator::stop()
{
    thread()->requestInterruption();
    thread()->wait();
}

void ImageGenerator::process()
{
    quint8 r = QRandomGenerator::global()->bounded(256);
    quint8 g = QRandomGenerator::global()->bounded(256);
    quint8 b = QRandomGenerator::global()->bounded(256);

    int row = 0;
    int col = 0;

    row = m_index % m_width;
    col = m_index / m_width;
    m_mutex.lock();
    m_img.setPixel(row, col, qRgb(r, g, b));
    m_mutex.unlock();
    m_index++;
    if(m_index == (m_width*m_height)){
        m_index = 0;
    }
    if(m_index % 10000 == 0)
    {
        qDebug() << m_et.elapsed();
        m_et.restart();
    }
    emit sigImageReady(&m_img, &m_mutex);
}

void ImageGenerator::threadFunc()
{
    while(!thread()->isInterruptionRequested())
    {
        process();
        QThread::msleep(10);
    }
    emit sigThreadFinished();
}
