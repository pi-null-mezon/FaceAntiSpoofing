#include "qtaskserver.h"

#include <QDataStream>
#include <QTcpSocket>

QTaskServer::QTaskServer(QObject *parent) : QObject(parent),
    qtcpclient(nullptr)
{
    connect(&qtcpsrv,SIGNAL(newConnection()),this,SLOT(enrollConnection()));
    connect(&qtcpsrv,&QTcpServer::acceptError,[&](){
        qDebug("TCP Server error: %s", qtcpsrv.errorString().toUtf8().constData());
    });
}

void QTaskServer::start(const QHostAddress &_addr, quint16 _port)
{
    if(qtcpsrv.listen(_addr,_port)) {
        qInfo("QTaskServer: start listening on %s:%u",_addr.toString().toUtf8().constData(),static_cast<uint>(_port));
    } else {
        qInfo("QTaskServer: can not start listening!");
    }
}

void QTaskServer::sendEncodedImage(const QByteArray &_encimg)
{
    if(qtcpclient) {
        QDataStream dso(qtcpclient);
        dso.setVersion(QDataStream::Qt_5_9);
        dso << static_cast<quint8>(replayattack::CommandCode::UploadImage);
        dso << static_cast<quint32>(_encimg.size());
        dso.writeRawData(_encimg.data(),_encimg.size());
        //dso << _encimg;
    }
}

void QTaskServer::reportAboutSessionStart()
{
    if(qtcpclient) {
        QDataStream dso(qtcpclient);
        dso.setVersion(QDataStream::Qt_5_9);
        dso << static_cast<quint8>(replayattack::CommandCode::StartSession);
    }
}

void QTaskServer::reportAboutSessionEnd()
{
    if(qtcpclient) {
        QDataStream dso(qtcpclient);
        dso.setVersion(QDataStream::Qt_5_9);
        dso << static_cast<quint8>(replayattack::CommandCode::EndSession);
    }
}

void QTaskServer::enrollConnection()
{
    if(!qtcpclient) {
        qtcpclient = qtcpsrv.nextPendingConnection();
        qInfo("QTaskServer: client connected, dscr: %d",static_cast<int>(qtcpclient->socketDescriptor()));
        connect(qtcpclient,SIGNAL(readyRead()),this,SLOT(readClientReply()));
        emit clientConnected();
    } else {
        QTcpSocket *_tmpsocket = qtcpsrv.nextPendingConnection();
        qInfo("QTaskServer: new client connection attempt, dscr: %d - will be refused",static_cast<int>(qtcpclient->socketDescriptor()));
        connect(_tmpsocket,SIGNAL(disconnected()),_tmpsocket,SLOT(deleteLater()));
        _tmpsocket->close();
    }
}

void QTaskServer::readClientReply()
{
    QTcpSocket *_tcpsocket = qobject_cast<QTcpSocket*>(sender());
    QDataStream dsi(_tcpsocket);
    dsi.setVersion(QDataStream::Qt_5_9);

    if(_tcpsocket->bytesAvailable() < sizeof(quint8))
        return;
    quint8 _reply;
    dsi >> _reply;
    qDebug("Reply from client: %u", (uint)_reply);
    switch(_reply) {
        case replayattack::ResultCode::ImageUpdated:
            emit imageUploaded();
            break;
        case replayattack::ResultCode::SessionPrepared:
            emit clientReady();
            break;
        case replayattack::ResultCode::SessionFinished:
            emit clientNotifiedAboutSessionEnd();
            break;
        case replayattack::ResultCode::InvalidImage:
            emit clientNotifyAboutInvalidImage();
            break;
    }
}
