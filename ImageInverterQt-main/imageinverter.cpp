#include "imageinverter.h"
#include <QThread>
#include <QDebug>
#include <QImage>
#include <QTime>
#include <QPixmap>
#include <QGraphicsScene>
#include <QRandomGenerator>

ImageInverter::ImageInverter(QImage img, int size, QObject *parent)
    : QObject{parent}
{
    m_img = img;
    m_width = img.width();
    m_height = img.height();
    m_size = size;
    m_index = 0;

    moveToThread(new QThread);
    connect(thread(), &QThread::started, this, &ImageInverter::threadFunc);
    connect(this, &ImageInverter::sigThreadFinished, thread(), &QThread::quit, Qt::DirectConnection);
}
ImageInverter::~ImageInverter()
{
    stop();
}

void ImageInverter::start()
{
    thread()->start();
}

void ImageInverter::stop()
{
    thread()->requestInterruption();
    thread()->wait();
}

void ImageInverter::process()
{
    int x = 0; int y = 0;

    x = m_index % m_width;
    y = m_index / m_width;
    m_index++;

    m_mutex.lock();
    for(int i=0; i<m_size; i++)
    {
        if((y+i)<m_height){
            QRgb tempColorRgb = m_img.pixel(x, y+i);
            QColor tempColor(tempColorRgb);
            tempColor.rgb();

            quint8 r = tempColor.red();
            quint8 g = tempColor.green();
            quint8 b = tempColor.blue();

            m_img.setPixel(x, y+i, qRgb(255-r, 255-g, 255-b));
        }
    }
    m_mutex.unlock();

    if(x == m_width - 1){
        m_index += (m_size - 1) * m_width;
    }
    if(m_index >= m_width * m_height){
        m_index = 0;
    }

    emit sigImageReady(&m_img, &m_mutex);
}

void ImageInverter::threadFunc()
{
    while(!thread()->isInterruptionRequested()){
        process();
        QThread::msleep(20);
    }
    emit sigThreadFinished();
}

