
#ifndef COUNTLINE_H
#define COUNTLINE_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QFile>

class countLine : public QObject
{
    Q_OBJECT

public:
    explicit countLine(QObject* parent = nullptr);
    ~countLine();

    int funcCountLines(const QString& fullPath);

};
#endif // COUNTLINE_H
