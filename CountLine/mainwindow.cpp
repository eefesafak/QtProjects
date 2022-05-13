#include "mainwindow.h"
#include "qheaderview.h"
#include "qtablewidget.h"
#include "ui_mainwindow.h"
#include "countline.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableWidget->setColumnWidth(0, 782);
    ui->tableWidget->setColumnWidth(1, 150);
    ui->tableWidget->setColumnWidth(2, 150);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addItemToList(const QString &file, const int &fileSize, const int &count)
{
    QTableWidgetItem *iFileName = new QTableWidgetItem(file);
    QTableWidgetItem *iFileSize = new QTableWidgetItem(QString::number(fileSize));
    QTableWidgetItem *iFileCount = new QTableWidgetItem(QString::number(count));

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
            if(file.fileName() == filee.fileName())
            {
                fileList.removeOne(filee);
            }
        }
    }

    for(const QFileInfo &subDirectory : qdir.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot))
    {
        fileList << getFileListFromDir(subDirectory.absoluteFilePath());
    }

    ui->lineEdit->setText(ui->lineEdit->displayText());
    return fileList;
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

    ui->tableWidget->setRowCount(0);
    QFileInfoList fileList = getFileListFromDir(path);

    int count = 0; int sum = 0;
    foreach(const QFileInfo& file, fileList)
    {
        count = m_ig->funcCountLines(file.filePath());
        addItemToList(file.filePath(), file.size(), count);
        sum += count;
    }
    ui->label->setText(QString::number(sum));
}
