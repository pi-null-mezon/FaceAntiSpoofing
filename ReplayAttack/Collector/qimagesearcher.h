#ifndef QIMAGESEARCHER_H
#define QIMAGESEARCHER_H

#include <QObject>
#include <QDir>

class QImageSearcher : public QObject
{
    Q_OBJECT
public:
    explicit QImageSearcher(QObject *parent = nullptr);
    void setDir(const QString &_dirname);

signals:
    void fileRead(const QByteArray &_encimg);
    void allFilesAlreadyEnrolled();

public slots:
    void readNextFile();

private:
    QDir dir;
    QStringList fileslist;
    int pos;
};

#endif // QIMAGESEARCHER_H
