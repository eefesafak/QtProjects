#include "countline.h"


countLine::countLine(QObject *parent)
    : QObject{parent}
{
}

countLine::~countLine()
{
}

int countLine::funcCountLines(const QString &fullPath)
{
    QFile file(fullPath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return -1;

    int counter = 0;
    while(!file.atEnd())
    {
        file.readLine();
        counter++;
    }

    return counter;
}
