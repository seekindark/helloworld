#include <QDebug>
#include "hudpproc.h"
#include "hsettings.h"


HUdpProc::HUdpProc(QObject *parent) : QObject(parent), m_udpsocket(this)
{
    qDebug(__FUNCTION__);

    bind_localAddr();

    connect(&m_udpsocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(on_sockStateChanged(QAbstractSocket::SocketState)));
    connect(&m_udpsocket, SIGNAL(readyRead()),
            this, SLOT(on_sockReadyRead()));
}

void HUdpProc::bind_localAddr()
{
    qDebug() << __FUNCTION__;

    QHostAddress localIp(g_settings->local_ip());
    quint16 localPort = quint16(g_settings->local_port().toInt());

    if(!m_udpsocket.bind(localIp, localPort))
    {
        qDebug() << __FUNCTION__ << "Failed to bind LocalAddr:" << localIp.toString() << localPort;
    }
    else
    {
        qDebug() << __FUNCTION__ << "OK to bind LocalAddr:" << localIp.toString() << localPort;
    }
    /*  UnconnectedState,
        HostLookupState,
        ConnectingState,
        ConnectedState,
        BoundState,
        ListeningState,
        ClosingState*/
    qDebug() << __FUNCTION__ <<"udpSocket.isValid="<<m_udpsocket.isValid()
                             << "state=" << m_udpsocket.state()
                             << "localIP ="<<m_udpsocket.localAddress().toString()
                            << "localPort = " <<m_udpsocket.localPort();



}
void HUdpProc::on_sockStateChanged(QAbstractSocket::SocketState state)
{
    qDebug() << __FUNCTION__ << state;

}

void HUdpProc::on_sockReadyRead()
{
    qDebug() << __FUNCTION__ << "bytesAvailable=" << m_udpsocket.bytesAvailable()
             << "pendingDatagramSize=" <<m_udpsocket.pendingDatagramSize();

    while(m_udpsocket.hasPendingDatagrams())
    {

        QNetworkDatagram datagram = m_udpsocket.receiveDatagram(m_udpsocket.pendingDatagramSize());
        qDebug() << __FUNCTION__ << "received data-size=" << datagram.data().size()
                 << "remote addr = " << datagram.senderAddress() << datagram.senderPort()
                 << "data="<<datagram.data();

        char reply=66;
        m_udpsocket.writeDatagram(&reply, sizeof(reply), datagram.senderAddress(), datagram.senderPort());

    }
}

void HUdpProc::on_localAddr_changed()
{
    qDebug(__FUNCTION__);
    m_udpsocket.close();
    bind_localAddr();

}


void HUdpProc::on_remoteChanged(int remoteId)
{
    qDebug() << __FUNCTION__ << "ID=" << remoteId;
}
