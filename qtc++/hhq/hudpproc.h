#ifndef HUDPPROC_H
#define HUDPPROC_H

#include <QObject>
#include <QtNetwork>
#include <QUdpSocket>
#include <QNetworkDatagram>


class HUdpProc : public QObject
{
    Q_OBJECT
public:
    explicit HUdpProc(QObject *parent = nullptr);


signals:

public slots:
    void on_localAddr_changed();
    void on_remoteChanged(int remoteId);
    void on_sockStateChanged(QAbstractSocket::SocketState);
    void on_sockReadyRead();

private:
    QUdpSocket m_udpsocket;

    void bind_localAddr();
};

extern HUdpProc *g_udpProc;

#endif // HUDPPROC_H
