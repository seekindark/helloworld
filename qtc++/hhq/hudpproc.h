#ifndef HUDPPROC_H
#define HUDPPROC_H

#include <QObject>
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

private:
        QUdpSocket m_udpsocket;

};

extern HUdpProc *g_udpProc;

#endif // HUDPPROC_H
