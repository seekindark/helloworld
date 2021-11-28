#include <QDebug>
#include "hudpproc.h"
#include "hsettings.h"
#include "hmessage.h"


HUdpProc::HUdpProc(QObject *parent) : QObject(parent), m_udpsocket(this)
{
    qDebug(__FUNCTION__);

    m_packet_tx = 0;
    m_packet_rx = 0;
    m_byte_rx = 0;
    m_byte_tx = 0;
    memset(m_rmpacket_tx, 0, sizeof(m_rmpacket_tx));
    memset(m_rmpacket_rx, 0, sizeof(m_rmpacket_rx));
    memset(m_rmbyte_tx, 0, sizeof(m_rmbyte_tx));
    memset(m_rmbyte_rx, 0, sizeof(m_rmbyte_rx));

    memset(m_rmtCfg, 0, sizeof(m_rmtCfg));
    memset(m_rmtStatus, 0, sizeof(m_rmtStatus));



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
    /*
    qDebug() << __FUNCTION__ << "bytesAvailable=" << m_udpsocket.bytesAvailable()
             << "pendingDatagramSize=" <<m_udpsocket.pendingDatagramSize()
             << "state=" << m_udpsocket.state();
*/
    int remoteID = 0;
    while(m_udpsocket.hasPendingDatagrams())
    {
        m_packet_rx += 1;
        m_byte_rx += m_udpsocket.pendingDatagramSize();

        QNetworkDatagram datagram = m_udpsocket.receiveDatagram(m_udpsocket.pendingDatagramSize());

        HMsgReply msg(datagram.data().data(), datagram.data().size());

        qDebug() << __FUNCTION__ << "received data-size=" << datagram.data().size()
                 << "remote addr = " << datagram.senderAddress() << datagram.senderPort();
                 //<< "data="<<datagram.data();

        remoteID= getRemoteId(datagram.senderAddress(),
                              datagram.data().size());
        sts_network(remoteID, datagram.data().size());
        sts_remote(remoteID, msg);
    }
}

//return the remote id 1 or 2
int HUdpProc::getRemoteId(const QHostAddress ip, int dataLen)
{
    for (int id=1; id<= 2; id++)
    {
        int idx = id - 1;
        QHostAddress remote_ip(g_settings->remote_ip(id));
        //
        // Note: the Remote is using UDP client mode, i.e. the sending udp port is random !!
        //       thus here, we don't need to check the port to identify the remote-id !
        //
        //int remote_port = g_settings->remote_port(i).toInt();
        if(remote_ip == ip /* && port == remote_port*/)
        {
            m_rmpacket_rx[idx] += 1;
            m_rmbyte_rx[idx] += dataLen;
            return id;
        }
    }

    return 0;
}

void HUdpProc::sts_network(int remoteId, int rcvPacketLen)
{
    if(isValidRemoteId(remoteId))
    {
        m_rmpacket_rx[_RmtID2Idx(remoteId)] += 1;
        m_rmbyte_rx[_RmtID2Idx(remoteId)] += rcvPacketLen;
    }
}
void HUdpProc::sts_remote(int remoteId, const HMsgReply &msg)
{
    if (isValidRemoteId(remoteId))
    {
        int idx = _RmtID2Idx(remoteId);
        m_rmtStatus[idx].emitter_temp = msg.emitter_temp();
        m_rmtStatus[idx].insulation_resistance = msg.insulation_resistance();
        m_rmtStatus[idx].powerMod1 = msg.powerMod1();
        m_rmtStatus[idx].powerMod2 = msg.powerMod2();
        m_rmtStatus[idx].powerAmp1 = msg.powerAmp1();
        m_rmtStatus[idx].powerAmp2 = msg.powerAmp2();
        m_rmtStatus[idx].avgOutVoltag_amp1 = msg.avgOutVoltag_amp1();
        m_rmtStatus[idx].avgOutVoltag_amp2 = msg.avgOutVoltag_amp2();
        m_rmtStatus[idx].avgOutVoltag_amp3 = msg.avgOutVoltag_amp3();
        m_rmtStatus[idx].avgOutVoltag_amp4 = msg.avgOutVoltag_amp4();

    }
}

void HUdpProc::send_to_remote(const char *data, int len, int remoteID)
{
    if(data == nullptr || len == 0)
    {
        qDebug() << "HUdpProc::" << __FUNCTION__ << "send zero data ! skip it !";
        return;
    }

    if(isValidRemoteId(remoteID))
    {
        int remoteIdx = _RmtID2Idx(remoteID);
        QHostAddress remote_ip(g_settings->remote_ip(remoteID));
        quint16 remote_port= quint16(g_settings->remote_port(remoteID).toInt());
        if(!remote_ip.isNull() && remote_port > 0)
        {
            m_udpsocket.writeDatagram(data, len, remote_ip, remote_port);
            m_rmbyte_tx[remoteIdx] += len;
            m_rmpacket_tx[remoteIdx] += 1;

            m_byte_tx += len;
            m_packet_tx+= 1;

        }
    }
    else
    {
        qDebug() << "HUdpProc::" << __FUNCTION__ << " invalid remoteId " << remoteID;
        return;
    }
}

void HUdpProc::send(const char *data, int len)
{
    if(data == nullptr || len == 0)
    {
        qDebug() << "HUdpProc::" << __FUNCTION__ << "send zero data ! skip it !";
        return;
    }
    //send to remote 1
    send_to_remote(data, len, 1);
    send_to_remote(data, len, 2);

}

void HUdpProc::sendHMsgReq(HMsgRequest &msg)
{
    sendHMsgReq_to_remote(msg, 1);
    sendHMsgReq_to_remote(msg, 2);

}
void HUdpProc::sendHMsgReq_to_remote(HMsgRequest &msg, int remoteId)
{
    if(isValidRemoteId(remoteId))
    {
        QHostAddress remote_ip(g_settings->remote_ip(remoteId));
        quint16 remote_port= quint16(g_settings->remote_port(remoteId).toInt());
        if(!remote_ip.isNull() && remote_port > 0)
        {

            //re-config the msg with settings
            if(g_settings->remote_enableMode_v(remoteId) == HSettings::e_enableMode_manual)
            {
                msg.setPowerMod1(g_settings->remote_powerMod1_v(remoteId));
                msg.setPowerMod2(g_settings->remote_powerMod2_v(remoteId));
                msg.setPowerAmp1(g_settings->remote_powerAmp1_v(remoteId));
                msg.setPowerAmp2(g_settings->remote_powerAmp2_v(remoteId));
                msg.setVolume(g_settings->remote_volume(remoteId));
            }

            //sendout
            m_udpsocket.writeDatagram(msg.toEncodedMsg(), msg.encodedMsgSize(),
                                      remote_ip, remote_port);

            //update staistics
            int remoteIdx = _RmtID2Idx(remoteId);
            m_rmbyte_tx[remoteIdx] += msg.encodedMsgSize();
            m_rmpacket_tx[remoteIdx] += 1;

            m_byte_tx += msg.encodedMsgSize();
            m_packet_tx+= 1;

        }
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

const QString HUdpProc::socketStatus()
{
    switch(m_udpsocket.state())
    {
    case QAbstractSocket::UnconnectedState:
        return "Unconnected";
    break;
    case QAbstractSocket::HostLookupState:
        return "HostLookup";
    break;
    case QAbstractSocket::ConnectingState:
        return "Connecting";
    break;
    case QAbstractSocket::ConnectedState:
        return "Connected";
    break;
    case QAbstractSocket::BoundState:
        return "Bound";
    break;
    case QAbstractSocket::ListeningState:
        return "Listening";
    break;
    case QAbstractSocket::ClosingState:
        return "Closing";
    break;
    }

    return "Error";
}

const QString HUdpProc::powerModState2Str(int id, int val)
{
    /*
     *  每位表示电源模块的一种故障状态，0-正常；1-故障：
        bit0=1：电源待机
        bit1=1：输入异常；
        bit2=1：输出异常
    */

    if(RMpacketRx(id) <= 0)
    {
        return "N/A";
    }

    QString str;
    if(val==0)            //CF: 后三位BIT全 0 表示正常
    {
        str ="Normal";
    }
    else
    {
        if(val & 0x01)
        {
            str += "Standby ";
        }
        if(val & 0x02)
        {
            if(str.length() > 0) str += ", ";
            str += "Input-Exception ";
        }
        if(val & 0x04)
        {
            if(str.length() > 0) str += ", ";
            str += "Output-Exception ";
        }
    }

    return str;
}

const QString HUdpProc::powerAmpState2Str(int id, int val)
{
    /*
    每位表示1个功放的一种故障状态，1-正常；0-故障：
    bit0=0：功放待机
    bit1=0：功放削波/过温；
    bit2=0：功放故障
    */

    if(RMpacketRx(id) <= 0)
    {
        return "N/A";
    }

    QString str = QString::number(val);
     if(val==0x7)    //CF: 后三位BIT全 1 表示正常
     {
         str = "Normal";
     }
     else
     {
         if((val & 0x01)==0)  //bit-0 = 0
         {
             str += "Standby ";
         }
         if((val & 0x02)==0)    //bit-1 = 0
         {
             if(str.length() > 0) str += ", ";
             str +=" Clipping/Overheating ";
         }
         if((val & 0x04)==0)    //bit-2 = 0
         {
             if(str.length() > 0) str += ", ";
             str += "Failure ";
         }
     }

     return str;
}

const QString HUdpProc::emitter_temp_str(int id)
{
    if(RMpacketRx(id) > 0)
    {
        int val = emitter_temp(id);
        if(val >=-55 && val <=175)
        {
            return QString::number(val);
        }
        else
        {
            return "Error";
        }
    }
    else
    {
        return "N/A";
    }
}
