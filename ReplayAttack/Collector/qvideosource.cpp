#include "qvideosource.h"

#include <QElapsedTimer>
#include <QEventLoop>

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

QVideoSource::QVideoSource(QObject *parent) : QObject(parent),
    devid(0)
{    
    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<QVideoSource::Error>("QVideoSource::Error");
}

QVideoSource::~QVideoSource()
{   
}

void QVideoSource::open()
{
    if(devid > -1) {
        if(cvvideocapture.open(devid)) {
            cvvideocapture.set(cv::CAP_PROP_FRAME_WIDTH, framesize.width);
            cvvideocapture.set(cv::CAP_PROP_FRAME_HEIGHT, framesize.height);
            timer->start();
            emit opened();
        } else {
            emit error(DeviceCanNotBeOpened);
            qDebug("QVideoSource - Can not open video device with id %d",devid);
        }
    }
}

void QVideoSource::_grabFrame()
{
    if(cvvideocapture.isOpened()) {
        cv::Mat _frame;
        if(cvvideocapture.read(_frame)) {
            cv::imshow(APP_NAME,_frame);
            if(cv::waitKey(1) == 's')
                cvvideocapture.set(CV_CAP_PROP_SETTINGS,0.0);
            emit frameUpdated(_frame);            
        }
    }
}

cv::Size QVideoSource::getFramesize() const
{
    return framesize;
}

void QVideoSource::setFramesize(const cv::Size &value)
{
    framesize = value;
}

int QVideoSource::getVideodevID() const
{
    return devid;
}

void QVideoSource::setVideodevID(int _id)
{
    //qDebug("QVideoSource::Debug - videoodevice id has been set to %d",_id);
    devid = _id;
}

void QVideoSource::close()
{
    if(cvvideocapture.isOpened()) {
        timer->stop();
        cvvideocapture.release();
    }
}

void QVideoSource::pause()
{
    if(cvvideocapture.isOpened()) {
        timer->stop();
    }
}

void QVideoSource::resume()
{
    if(cvvideocapture.isOpened()) {
        timer->start();
    }
}

void QVideoSource::measureFPS()
{
    QElapsedTimer _qeltimer;
    double _elapsedms = 0;
    uint  _frame = 0, _frames = 0;
    auto _moconn = connect(this, &QVideoSource::frameUpdated, [&]() {
        _frame++;
        if(_frame == 1) {
            _qeltimer.start();
        } else {
            _frames++;
            _elapsedms = _qeltimer.elapsed();
        }
    });
    QEventLoop _el;
    QTimer::singleShot(3000,&_el,SLOT(quit()));
    _el.exec();
    disconnect(_moconn);
    double _actualfps = 1000.0 * _frames / _elapsedms;
    qDebug("QVideoSource - fps measured %.2f", _actualfps);
    if((std::isinf(_actualfps) == false) && (std::isnan(_actualfps) == false) && (_actualfps > 10.0)) {
        emit fpsMeasured(_actualfps);
    } else {
        emit error(InvalidFPS);
    }
}

void QVideoSource::init()
{
    timer = new QTimer();
    timer->setTimerType(Qt::PreciseTimer);
    timer->setInterval(30.0);
    connect(timer, SIGNAL(timeout()), this, SLOT(_grabFrame()));
}
