#include "qtcpclient.h"

#include <QDataStream>

QTCPClient::QTCPClient(QObject *parent) : QObject(parent),
    imgsizebytes(0),   
    commandcode(replayattack::CommandCode::Unknown)
{
    connect(&tcpsocket,SIGNAL(readyRead()),this,SLOT(readSocket()));
}

void QTCPClient::setHostAddr(QHostAddress _addr, quint16 _port)
{
    hostaddr = _addr;
    hostport = _port;
}

void QTCPClient::connectToHost()
{
    tcpsocket.connectToHost(hostaddr,hostport);
}

void QTCPClient::notifyAboutImageUpdate()
{
    commandcode = replayattack::Unknown;
    imgsizebytes = 0;
    qDebug("  send notification about img update to the server");
    QDataStream ods(&tcpsocket);
    ods.setVersion(QDataStream::Qt_5_0);
    ods << static_cast<quint8>(replayattack::ResultCode::ImageUpdated);
}

void QTCPClient::notifyAboutStart()
{
    commandcode = replayattack::Unknown;
    QDataStream ods(&tcpsocket);
    ods.setVersion(QDataStream::Qt_5_0);
    ods << static_cast<quint8>(replayattack::ResultCode::SessionPrepared);
}

void QTCPClient::notifyAboutEnd()
{
    commandcode = replayattack::Unknown;
    QDataStream ods(&tcpsocket);
    ods.setVersion(QDataStream::Qt_5_0);
    ods << static_cast<quint8>(replayattack::ResultCode::SessionFinished);
}

void QTCPClient::notifyAboutInvalidImage()
{
    commandcode = replayattack::Unknown;
    imgsizebytes = 0;
    qDebug("  send notification about invalid img to the server");
    QDataStream ods(&tcpsocket);
    ods.setVersion(QDataStream::Qt_5_0);
    ods << static_cast<quint8>(replayattack::ResultCode::InvalidImage);
}

void QTCPClient::readSocket()
{   
    //qDebug("  readSocket(): bytes available %u", (uint)tcpsocket.bytesAvailable());
    QDataStream ids(&tcpsocket);
    ids.setVersion(QDataStream::Qt_5_0);

    if(commandcode == replayattack::CommandCode::Unknown) {
        if(tcpsocket.bytesAvailable() < sizeof(quint8))
            return;
        ids >> commandcode;
        //qDebug("  Server command: %u",(uint)commandcode);
    }

    switch(commandcode) {
        case replayattack::UploadImage: {
            if(imgsizebytes == 0) {
                if(tcpsocket.bytesAvailable() < sizeof(quint32))
                    return;
                ids >> imgsizebytes;
                qDebug("  Imgsize: %u",(uint)imgsizebytes);
            }

            if((tcpsocket.bytesAvailable() < imgsizebytes) && (imgsizebytes != 0))
                return;

            QByteArray _encimg;
            _encimg.resize(imgsizebytes);
            ids.readRawData(_encimg.data(),_encimg.size());
            qDebug("Image recieved, size: %u (bytes left in socket %u)",(uint)_encimg.size(),(uint)tcpsocket.bytesAvailable());
            emit imageRecieved(_encimg);

        } break;

        case replayattack::StartSession:
            emit startSessionCommand();
            notifyAboutStart();
            break;

        case replayattack::EndSession:
            emit endSessionCommand();
            notifyAboutEnd();
            break;
    }
}
