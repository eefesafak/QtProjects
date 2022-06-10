#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "countline.h"
#include <QMainWindow>
#include <QPushButton>
#include <QFileInfoList>
#include <QFileDialog>
#include <QtConcurrentRun>
#include <QMutex>
#include <QElapsedTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QFileInfoList getFileListFromDir(const QString& directory);
    void process(const QString& path);
    bool getReturnState();
    void setReturnState(bool state);
    void SaveSettings();
    void LoadSettings();

signals:
    void processFinished(int sum, int totalFiles);
    void addItemToList(const QString& file, const int& fileSize, const int& count);

public slots:
    void slotProcessFinished(int sum, int totalFiles);
    void slotAddItemToList(const QString& file, const int& fileSize, const int& count);

private:
    Ui::MainWindow *ui;
    bool m_return;
    QMutex m_mutex;
    countLine* m_ig;
    QElapsedTimer m_timer;
private slots:
    void on_Browse_clicked();
    void on_Ok_clicked();
    void on_cancel_clicked();
};
#endif // MAINWINDOW_H
