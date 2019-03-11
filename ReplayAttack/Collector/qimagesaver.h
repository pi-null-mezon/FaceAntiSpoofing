#ifndef QIMAGESAVER_H
#define QIMAGESAVER_H

#include <QObject>
#include <QDir>

#include <opencv2/core.hpp>

class QImageSaver : public QObject
{
    Q_OBJECT
public:
    explicit QImageSaver(QObject *parent = nullptr);
    void setDir(const QString &_dirname);

signals:
    void imageSaved();

public slots:
    void updateImage(const cv::Mat &_img);
    void saveImage();

private:
    QDir dir;
    cv::Mat img;
};

#endif // QIMAGESAVER_H
