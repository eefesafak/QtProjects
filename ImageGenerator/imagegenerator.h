#ifndef IMAGEGENERATOR_H
#define IMAGEGENERATOR_H

#include <QObject>
#include <QImage>
#include <QElapsedTimer>
#include <QMutex>
class ImageGenerator : public QObject
{
    Q_OBJECT
public:
    explicit ImageGenerator(int width, int height, QObject *parent = 0);
    ~ImageGenerator();

    void start();
    void stop();

private:
    void process();
    QImage m_img;
    int m_index;
    int m_width, m_height;
    QElapsedTimer m_et;
    QMutex m_mutex;
signals:
    void sigImageReady(QImage *img, QMutex *mutex);
    void sigThreadFinished();
    void sigTest();

public slots:
    void threadFunc();
};

#endif // IMAGEGENERATOR_H
