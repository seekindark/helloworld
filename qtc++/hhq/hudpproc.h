#ifndef HUDPPROC_H
#define HUDPPROC_H

#include <QObject>
#include <QtNetwork>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include "hmessage.h"


class HUdpProc : public QObject
{
    Q_OBJECT
public:

    typedef struct
    {
        bool is_manualMode;
        bool powerMod1;
        bool powerMod2;
        bool powerAmp1;
        bool powerAmp2;
    }T_RemoteCfg;

    typedef struct
    {
        //net work
        /*
        qint64 packet_tx;
        qint64 packet_rx;
        qint64 byte_tx;
        qint64 byte_rx;
        */
        //Emitter and Electronic Bay
        /*
         * 10~240， 对应―55~175℃; value = data-65 C
        */
        int emitter_temp;
        /*
            0~255，对应0~10MΩ
            Value = data * 40(kΩ)
        */
        int insulation_resistance;

        //moduel working mode
        /*
         *  每位表示电源模块的一种故障状态，0-正常；1-故障：
            bit0=1：电源待机
            bit1=1：输入异常；
            bit2=1：输出异常
        */
        int powerMod1;
        int powerMod2;
        /*
        每位表示1个功放的一种故障状态，1-正常；0-故障：
        bit0=0：功放待机
        bit1=0：功放削波/过温；
        bit2=0：功放故障
        */
        int powerAmp1;
        int powerAmp2;

        //average voltage output
        /*
            0~255，
            对应0~255V
        */
        int avgOutVoltag_amp1;
        int avgOutVoltag_amp2;
        int avgOutVoltag_amp3;
        int avgOutVoltag_amp4;

    }T_RemoteStatus;


    explicit HUdpProc(QObject *parent = nullptr);
    void bind_localAddr();

    void send(const char *data, int len);
    void sendHMsgReq(HMsgRequest &msg);

    const QString socketStatus();
    qint64 packetTx(){return m_packet_tx;}
    qint64 packetRx(){return m_packet_rx;}
    qint64 bytesTx(){return m_byte_tx;}
    qint64 bytesRx(){return m_byte_rx;}


    qint64 RMpacketTx(int id){ return (isValidRemoteId(id)?m_rmpacket_tx[id-1] : 0);}
    qint64 RMpacketRx(int id){ return (isValidRemoteId(id)?m_rmpacket_rx[id-1] : 0);}
    qint64 RMbytesTx(int id){return  (isValidRemoteId(id)?m_rmbyte_tx[id-1] : 0);}
    qint64 RMbytesRx(int id){return  (isValidRemoteId(id)?m_rmbyte_rx[id-1] : 0);}

    int emitter_temp(int id){return m_rmtStatus[id-1].emitter_temp;}
    const QString emitter_temp_str(int id);
    int insulation_resistance(int id){return m_rmtStatus[id-1].insulation_resistance;}
    QString powerMod1(int id){return powerModState2Str(id, m_rmtStatus[id-1].powerMod1);}
    QString powerMod2(int id){return powerModState2Str(id, m_rmtStatus[id-1].powerMod2);}
    QString powerAmp1(int id){return powerAmpState2Str(id, m_rmtStatus[id-1].powerAmp1);}
    QString powerAmp2(int id){return powerAmpState2Str(id, m_rmtStatus[id-1].powerAmp2);}

    int avgOutVoltag_amp1(int id){return m_rmtStatus[id-1].avgOutVoltag_amp1;}
    int avgOutVoltag_amp2(int id){return m_rmtStatus[id-1].avgOutVoltag_amp2;}
    int avgOutVoltag_amp3(int id){return m_rmtStatus[id-1].avgOutVoltag_amp3;}
    int avgOutVoltag_amp4(int id){return m_rmtStatus[id-1].avgOutVoltag_amp4;}

    const QString powerModState2Str(int id, int val);
    const QString powerAmpState2Str(int id, int val);


signals:

public slots:
    void on_localAddr_changed();
    void on_remoteChanged(int remoteId);
    void on_sockStateChanged(QAbstractSocket::SocketState);
    void on_sockReadyRead();

private:
    QUdpSocket m_udpsocket;

    qint64 m_packet_tx;
    qint64 m_packet_rx;
    qint64 m_byte_tx;
    qint64 m_byte_rx;

    qint64 m_rmpacket_tx[2];
    qint64 m_rmpacket_rx[2];
    qint64 m_rmbyte_tx[2];
    qint64 m_rmbyte_rx[2];


    T_RemoteCfg m_rmtCfg[2];
    T_RemoteStatus m_rmtStatus[2];

    void send_to_remote(const char *data, int len, int remoteID);
    void sendHMsgReq_to_remote(HMsgRequest &msg, int remoteId);
    void sts_network(int remoteId, int rcvPacketLen);
    void sts_remote(int remoteId, const HMsgReply &msg);

    int getRemoteId(const QHostAddress ip, int dataLen);
    bool isValidRemoteId(int remoteId) {return (remoteId >=1 && remoteId <=2);}
    int _RmtID2Idx(int id){ return id - 1;}
};

extern HUdpProc *g_udpProc;

#endif // HUDPPROC_H
