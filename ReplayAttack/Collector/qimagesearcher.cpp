#include "qimagesearcher.h"
#include <QFile>

QImageSearcher::QImageSearcher(QObject *parent) : QObject(parent)
{
}

void QImageSearcher::setDir(const QString &_dirname)
{
    dir.setPath(_dirname);
    QStringList filefilters;
    filefilters << "*.jpg" << "*.jpeg" << "*.png";
    fileslist = dir.entryList(filefilters,QDir::Files | QDir::NoDotAndDotDot);
    qInfo("QImageSearcher: %d files have been found", fileslist.size());
    pos = 0;
}

void QImageSearcher::readNextFile()
{
    if((fileslist.size() > 0) && (pos < fileslist.size())) {
        qInfo("  %d) %s", pos, dir.absoluteFilePath(fileslist.at(pos)).toUtf8().constData());
        QFile _file(dir.absoluteFilePath(fileslist.at(pos)));
        _file.open(QIODevice::ReadOnly);
        emit fileRead(_file.readAll());
        pos++;
    } else {
        emit allFilesAlreadyEnrolled();
    }
}
