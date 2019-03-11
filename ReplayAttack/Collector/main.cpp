#include <QCoreApplication>
#include <QThread>
#include <QDir>

#include <opencv2/core.hpp>

#include "qtaskserver.h"
#include "qimagesearcher.h"
#include "qvideosource.h"
#include "qimagesaver.h"

const cv::String options = "{device v | 0       | identifier of the video device to be used}"
                           "{width    | 640     | target image width}"
                           "{height   | 480     | target image height}"
                           "{input i  |         | input dir to be processed}"
                           "{output o |         | otput dir to be used}"
                           "{addr a   | 0.0.0.0 | ip addres to listen on}"
                           "{port p   | 8000    | port to listen on}"
                           "{help h   |         | show help}";

int main(int argc, char **argv)
{
#ifdef Q_OS_WIN
    setlocale(LC_CTYPE,"Rus");
#endif
    cv::CommandLineParser cmdparser(argc,argv,options);
    cmdparser.about("This app was designed to collect replay attacks");
    if(argc == 1 || cmdparser.has("help")) {
        cmdparser.printMessage();
        return 0;
    }
    if(!cmdparser.has("input")) {
        qInfo("No input directory provided! Abort...");
        return 1;
    }
    QDir idir(cmdparser.get<std::string>("input").c_str());
    if(!idir.exists()) {
        qInfo("Input directory '%s' does not exist! Abort...",idir.absolutePath().toUtf8().constData());
        return 2;
    }
    if(!cmdparser.has("output")) {
        qInfo("No output directory provided! Abort...");
        return 3;
    }
    QDir odir(cmdparser.get<std::string>("output").c_str());
    if(!odir.exists()) {
        if(odir.mkpath(odir.absolutePath())) {
            qInfo("Output directory '%s' has been created",odir.absolutePath().toUtf8().constData());
        } else {
            qInfo("Output directory '%s' can not be created",odir.absolutePath().toUtf8().constData());
            return 4;
        }
    }

    QCoreApplication app(argc,argv);

    QVideoSource *videosource = new QVideoSource();
    videosource->setVideodevID(cmdparser.get<int>("device"));
    videosource->setFramesize(cv::Size(cmdparser.get<int>("width"),cmdparser.get<int>("height")));
    QThread *videosourcethread = new QThread(&app);
    QObject::connect(videosourcethread,SIGNAL(started()),videosource,SLOT(init()));
    QObject::connect(videosourcethread,SIGNAL(finished()),videosource,SLOT(deleteLater()));
    QObject::connect(videosourcethread,SIGNAL(finished()),&app,SLOT(quit()));
    videosourcethread->start();

    QImageSearcher imgsearcher;
    imgsearcher.setDir(idir.absolutePath());        

    QImageSaver imgsaver;
    imgsaver.setDir(odir.absolutePath());
    QObject::connect(videosource,SIGNAL(frameUpdated(cv::Mat)),&imgsaver,SLOT(updateImage(cv::Mat)));

    QTaskServer server;
    server.start(QHostAddress(cmdparser.get<std::string>("addr").c_str()),static_cast<quint16>(cmdparser.get<int>("port")));

    QObject::connect(&server,SIGNAL(clientConnected()),&server,SLOT(reportAboutSessionStart()));
    QObject::connect(&server,SIGNAL(clientReady()),&imgsearcher,SLOT(readNextFile()));
    QObject::connect(&imgsearcher,SIGNAL(fileRead(QByteArray)),&server,SLOT(sendEncodedImage(QByteArray)));
    QObject::connect(&server,SIGNAL(imageUploaded()),&imgsaver,SLOT(saveImage()));
    QObject::connect(&server,SIGNAL(clientNotifyAboutInvalidImage()),&imgsearcher,SLOT(readNextFile()));
    QObject::connect(&imgsaver,SIGNAL(imageSaved()),&imgsearcher,SLOT(readNextFile()));
    QObject::connect(&imgsearcher,SIGNAL(allFilesAlreadyEnrolled()),&server,SLOT(reportAboutSessionEnd()));
    QObject::connect(&server,SIGNAL(clientNotifiedAboutSessionEnd()),videosourcethread,SLOT(quit()));    

    QTimer::singleShot(0,videosource,SLOT(open()));
    return app.exec();
}
