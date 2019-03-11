#ifndef QTASKSERVER_H
#define QTASKSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QHostAddress>

#include <QByteArray>

#include "protocolstuff.h"

class QTaskServer : public QObject
{
    Q_OBJECT
public:
    explicit QTaskServer(QObject *parent = nullptr);
    void start(const QHostAddress &_addr, quint16 _port);

signals:
    void imageUploaded();
    void clientConnected();
    void clientReady();
    void clientNotifiedAboutSessionEnd();
    void clientNotifyAboutInvalidImage();

public slots:
    void sendEncodedImage(const QByteArray &_encimg);
    void reportAboutSessionStart();
    void reportAboutSessionEnd();

private slots:
    void enrollConnection();
    void readClientReply();

private:
    QTcpServer qtcpsrv;
    QTcpSocket *qtcpclient;
};

#endif // QTASKSERVER_H
