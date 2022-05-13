#ifndef IMAGEINVERTER_H
#define IMAGEINVERTER_H

#include <QObject>
#include <QImage>
#include <QElapsedTimer>
#include <QMutex>

class ImageInverter : public QObject
{
    Q_OBJECT
public:
    explicit ImageInverter(QImage img, int size, QObject *parent = nullptr);
    ~ImageInverter();

    void start();
    void stop();
private:
    void process();
    QImage m_img;
    int m_size;
    int m_index;
    int m_width; int m_height;
    QElapsedTimer m_et;
    QMutex m_mutex;
signals:
    void sigImageReady(QImage *img, QMutex *mutex);
    void sigThreadFinished();
    void sigTest();

public slots:
    void threadFunc();
};

#endif // IMAGEINVERTER_H
