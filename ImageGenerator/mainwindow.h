#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "imagegenerator.h"
//#include <QMutex>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    ImageGenerator* m_ig;

public slots:
    void slotImage(QImage *img, QMutex *mutex);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
