#ifndef QTCPCLIENT_H
#define QTCPCLIENT_H

#include <QObject>

#include <QTcpSocket>
#include <QHostAddress>

#include "protocolstuff.h"

class QTCPClient : public QObject
{
    Q_OBJECT
public:
    explicit QTCPClient(QObject *parent = nullptr);
    void setHostAddr(QHostAddress _addr, quint16 _port);

signals:
    void imageRecieved(const QByteArray &_encimg);
    void endSessionCommand();
    void startSessionCommand();

public slots:
    void connectToHost();
    void notifyAboutImageUpdate();
    void notifyAboutStart();
    void notifyAboutEnd();
    void notifyAboutInvalidImage();

private slots:
    void readSocket();

private:
    QTcpSocket   tcpsocket;
    QHostAddress hostaddr;
    quint16      hostport;

    quint32 imgsizebytes;
    quint8  commandcode;
};

#endif // QTCPCLIENT_H
