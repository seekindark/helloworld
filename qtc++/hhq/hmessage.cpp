#include <QDebug>
#include "hmessage.h"

HMsgRequest::HMsgRequest(const char *data, int len)
{
    //qDebug(__FUNCTION__);
    static int m_maxLength = _maxDataSize();
    memset(&m_msg, 0, sizeof(m_msg));

    m_msg.messageHeader = 0xAA34;
    m_msg.messageLengthLow = 0x2238;
    m_msg.messageLengthHigh = 0;
    /*
    0x1001：系统显控台，PC机
    0x5001：喊话器1语音控制盒
    0x5002：喊话器1发射仓
    0x5011：喊话器2语音控制盒
    0x5012：喊话器2发射仓
    */
    m_msg.sendID = 0x1001;          //?? FixME ??
    m_msg.receiveID = 0x5002;
    m_msg.messageType = 0;
    m_msg.identifyCode = 0x34;
    /*
    1100：麦克风模式；
    1200：线路输入模式；
    1400：预存语音模式，循环播放语音片段1~10；
    1441~144A：重复播放选择的语音片段。1-第1段，A-第10段。
    1451~145A：录制语音片段至存储位置1~10。1-第1段，A-第10段。
    1800：远程控制模式；
    */
    m_msg.modeSetting = 0x1100;     // ?? FIXME ??
    m_msg.stateSetting = 0x330F;       // it's updated later by reading config
    m_msg.volumeAdjustment = 0x6607; // it's updated later by reading config

    if(data != nullptr && len > 0)
    {
        if(len > m_maxLength)
        {
            len = m_maxLength;
        }
        m_msg.backUp = len;              // it's not used. But we can fill it as Length of data

        memcpy(m_msg.voiceData, data, len);
    }
    m_msg.backUpData[0] = 0;
    m_msg.messageTrailer = 0x34AA;
}

void HMsgRequest::setData(const char *data, int len)
{
    if(data != nullptr && len > 0 && len <= _maxDataSize())
    {
        memcpy(m_msg.voiceData, data, len);
        setDataLen(len);
    }
}



HMsgReply::HMsgReply(const char *data, int len)
{
    //qDebug(__FUNCTION__);

    m_msg = nullptr;

    if(len != sizeof(T_MsgReply))
    {
        qDebug() << __FUNCTION__ << "data len=" << len << "is not equal to msgReplySize=" << sizeof(T_MsgReply);
    }

    m_msg = (T_MsgReply *) data;

}
