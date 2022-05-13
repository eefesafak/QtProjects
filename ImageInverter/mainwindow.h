#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "imageinverter.h"

#include <QMainWindow>
#include <QMutex>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    ImageInverter *m_ig;
public slots:
    void slotImage(QImage *img, QMutex *mutex);
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
