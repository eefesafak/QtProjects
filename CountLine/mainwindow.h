#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "countline.h"
#include <QMainWindow>
#include <QPushButton>
#include <QFileInfoList>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void addItemToList(const QString& file, const int& fileSize, const int& count);
    QFileInfoList getFileListFromDir(const QString& directory);

private slots:
    void on_Browse_clicked();
    void on_Ok_clicked();

private:
    Ui::MainWindow *ui;
    countLine* m_ig;
};
#endif // MAINWINDOW_H
