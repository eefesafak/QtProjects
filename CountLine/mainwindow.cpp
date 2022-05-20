#include "mainwindow.h"
#include "qheaderview.h"
#include "qtablewidget.h"
#include "ui_mainwindow.h"
#include "countline.h"
#include <QtConcurrentRun>
#include <QMutexLocker>
#include <unistd.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setReturnState(false);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    ui->tableWidget->setColumnWidth(0, 782);
    ui->tableWidget->setColumnWidth(1, 150);
    ui->tableWidget->setColumnWidth(2, 150);

    connect(this, &MainWindow::processFinished, this, &MainWindow::slotProcessFinished);
    connect(this, &MainWindow::addItemToList, this, &MainWindow::slotAddItemToList);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotAddItemToList(const QString &file, const int &fileSize, const int &count)
{
    QTableWidgetItem *iFileName = new QTableWidgetItem;
    QTableWidgetItem *iFileSize = new QTableWidgetItem;
    QTableWidgetItem *iFileCount = new QTableWidgetItem;

    iFileName->setData(Qt::EditRole, file);
    iFileSize->setData(Qt::EditRole, fileSize);
    iFileCount->setData(Qt::EditRole, count);

    int rowCount = ui->tableWidget->rowCount();

    ui->tableWidget->insertRow(rowCount);

    rowCount = ui->tableWidget->rowCount();

    ui->tableWidget->setItem(rowCount-1, 0, iFileName);
    ui->tableWidget->setItem(rowCount-1, 1, iFileSize);
    ui->tableWidget->setItem(rowCount-1, 2, iFileCount);
}

QFileInfoList MainWindow::getFileListFromDir(const QString &directory)
{
    QDir qdir(directory);

    QString include = ui->lineEdit->text();
    QString except = ui->lineEdit2->text();

    QStringList includeList = include.split(QLatin1Char(';'));
    QStringList exceptList = except.split(QLatin1Char(';'));

    QFileInfoList fileList = qdir.entryInfoList(QStringList() << includeList, QDir::Files, QDir::Size);

    for(const QFileInfo& file : qdir.entryInfoList(QStringList() << exceptList))
    {
        for(const QFileInfo& filee: fileList)
        {
            if(getReturnState())
                break;
            if(file.fileName() == filee.fileName())
                fileList.removeOne(filee);
        }
    }

    for(const QFileInfo &subDirectory : qdir.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot))
    {
        if(getReturnState())
            break;
        fileList << getFileListFromDir(subDirectory.absoluteFilePath());
    }

    return fileList;    
}

void MainWindow::process(const QString& path)
{
    QFileInfoList fileList = getFileListFromDir(path);

    int count = 0; int sum = 0; int totalFiles = fileList.count();
    foreach(const QFileInfo& file, fileList)
    {
        if(getReturnState())
            break;
        count = m_ig->funcCountLines(file.filePath());
        sum += count;
        emit addItemToList(file.filePath(), file.size(), count);
    }
    if(m_return)
    {
        totalFiles = 0;
    }
    setReturnState(false);

    emit processFinished(sum, totalFiles);
}

bool MainWindow::getReturnState()
{
    QMutexLocker locker(&m_mutex);
    return m_return;
}

void MainWindow::setReturnState(bool state)
{
    QMutexLocker locker(&m_mutex);
    m_return = state;
}

void MainWindow::slotProcessFinished(int sum, int totalFile)
{
    ui->label_6->setText(QString::number(totalFile));
    ui->label->setText(QString::number(sum));
    ui->Ok->setEnabled(true);
    ui->cancel->setEnabled(false);
}

void MainWindow::on_Browse_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home/efe/Desktop/build-SatirSay-Unnamed2-Release", QFileDialog::ShowDirsOnly |
                                                                                                                                      QFileDialog::DontResolveSymlinks);
    if(path.isEmpty())
        return;

    ui->FullPath->setText(path);
}

void MainWindow::on_Ok_clicked()
{
    QString path = ui->FullPath->text();
    if(path.isEmpty())
        return;

    ui->Ok->setEnabled(false);
    ui->cancel->setEnabled(true);
    ui->tableWidget->setRowCount(0);

    QtConcurrent::run(this, &MainWindow::process, path);
}

void MainWindow::on_cancel_clicked()
{
    m_return = true;
}

