#include "qimagesaver.h"

#include <QUuid>
#include <QTimer>

#include <opencv2/imgcodecs.hpp>

QImageSaver::QImageSaver(QObject *parent) : QObject(parent)
{

}

void QImageSaver::setDir(const QString &_dirname)
{
    dir.setPath(_dirname);
}

void QImageSaver::updateImage(const cv::Mat &_img)
{
    img = _img;
}

void QImageSaver::saveImage()
{
    if(!img.empty()) {
        QString _filename = dir.absolutePath().append("/%1.jpg").arg(QUuid::createUuid().toString());
        cv::imwrite(_filename.toStdString(),img);
        emit imageSaved();
    }
}
