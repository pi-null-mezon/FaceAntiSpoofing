#ifndef QVIDEOSOURCE_H
#define QVIDEOSOURCE_H

#include <QObject>
#include <QTimer>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

class QVideoSource : public QObject
{
    Q_OBJECT
public:
    enum Error {DeviceCanNotBeOpened, InvalidFPS};
    explicit QVideoSource(QObject *parent = nullptr);
    ~QVideoSource();

    int  getVideodevID() const;
    void setVideodevID(int _id);

    cv::Size getFramesize() const;
    void setFramesize(const cv::Size &value);

signals:
    void frameUpdated(const cv::Mat &_cvmat);
    void fpsMeasured(double _fps);
    void opened();
    void error(QVideoSource::Error _error);

public slots:
    void measureFPS();
    void open();
    void pause();
    void resume();
    void close();
    void init();

private slots:
    void _grabFrame();

private:
    QTimer *timer;
    cv::VideoCapture cvvideocapture;
    int devid;
    cv::Size framesize;
};


#endif // QVIDEOSOURCE_H
