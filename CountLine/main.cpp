#include "mainwindow.h"
#include "countline.h"
#include <QApplication>
#include <QSettings>
#include <QProgressDialog>
#include <QIcon>
#include <QtConfig>
#include <QAction>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    a.setWindowIcon(QIcon(":/efe.png"));

    w.show();
    return a.exec();
}
