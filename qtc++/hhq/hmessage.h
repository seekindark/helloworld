#ifndef HMESSAGE_H
#define HMESSAGE_H




typedef struct {                                      //编码
    unsigned short messageHeader;//报文头 0xAA34
    unsigned short messageLengthLow;//报文长度低位 0x2328
    unsigned short messageLengthHigh;//报文长度高位 0x0000
    unsigned short sendID;//发送ID 0x50x1     0x1001：系统显控台，PC 机;0x5001：喊话器 1 语音控制盒;0x5002：喊话器 1 发射仓;0x5011：喊话器 2 语音控制盒;0x5012：喊话器 2 发射仓
    unsigned short receiveID;//接收ID 0x50x2 同上
    unsigned char messageType;//报文类型 备用
    unsigned char identifyCode;//识别码 0x34 备用
    unsigned short modeSetting;//设备工作模式设置 0000~FFFF 1100：麦克风模式； 1200：线路输入模式； 1400：预存语音模式，循环播放语音片段 1~10； 1441~144A：重复播放选择的语音片段。1- 第 1 段，A-第 10 段。 1451~145A：录制语音片段至存储位置 1~10。 1-第 1 段，A-第 10 段。 1800：远程控制模式；
    unsigned short stateSetting;//模块工作状态设置 0x3300~330F 每位表示湿端模块的一种工作状态的切换： bit0=0：电源模块1使能; bit1=0：电源模块2使能； bit2=1：功放模块1使能； bit3=1：功放模块2使能；
    unsigned short volumeAdjustment;//音量调节 0x6600~660F 6600~660A：音量调节 0~10 级； 其余：静音
    unsigned short backUp;//备用
    unsigned char voiceData[8970];//语音数据 暂定，采样率 44kHz，数据 16bit，每包语音数据长度固定 0.1s，数据传输时间小于 0.1s；
    unsigned char backUpData[8];//备用
    unsigned short messageTrailer;//报文尾 0x34AA
}T_MsgRequest;

typedef struct {                                      //解码
    unsigned short messageHeader;//报文头 0xAA37
    unsigned short messageLengthLow;//报文长度低位 0x0258
    unsigned short messageLengthHigh;//报文长度高位 0x0000
    unsigned short sendID;//发送ID 0x50x2 0x1001：系统显控台，PC 机；0x5001：喊话器 1 语音控制盒；0x5002：喊话器 1 发射仓；0x5011：喊话器 2 语音控制盒；0x5012：喊话器 2 发射仓
    unsigned short receiveID;//接收ID  0xx0x1 同上
    unsigned char messageType;//报文类型 备用
    unsigned char workingMode;//工作模式 A0~A3 bit0=1：麦克风模式； bit1=1：线路输入模式； bit2=1：预存语音模式； bit3=1：远程控制模式；
    unsigned char launchTemperature;//发射仓温度 10~240，对应―55~175℃；Value = data – 65(℃)
    unsigned char launchHumidity;// 发射仓湿度0~100，对应 0~100％ 备用
    unsigned char launchPressure;//发射仓压力 0~100，对应 0~1MPa 备用
    unsigned char insulationResistance;//电子仓绝缘电阻值0~255， 对应 0~10MΩ Value = data * 40(kΩ)
    unsigned char ampModuleStatus1;//功放模块1工作状态 F0~F7；每位表示 1 个功放的一种故障状态， 1-正常；0-故障： bit0=0：功放待机 bit1=0：功放削波/过温； bit2=0：功放故障；
    unsigned char ampModuleStatus2;//功放模块2工作状态  F0~F3 同上
    unsigned char powerModuleState1;//电源模块1工作状态； 50~57 每位表示电源模块的一种故障状态， 0-正常；1-故障： bit0=1：电源待机 bit1=1：输入异常； bit2=1：输出异常；
    unsigned char powerModuleState2;//电源模块2工作状态；  50~57 同上
    unsigned char emptyData;//空
    unsigned char inputVoltage;//输入电压 1V 	84~172， 对应 176~264V 备用 Value = data + 92(V)
    unsigned char positiveVoltage;//功放电源正电压 0~255，对应 0~255V 备用
    unsigned char negativeVoltage;//功放电源负电压 0~255，对应 0~255V 备用
    unsigned char outputVoltage;//信号源输出电压 0~250， 对应 0~5V 备用 Value = data*0.02 (V)
    unsigned char firstOutputVoltage;//第1路功放输出电压 0~255，对应 0~255V Value = data (V)
    unsigned char secondOutputVoltage;//第2路功放输出电压 0~255，对应 0~255V Value = data (V)
    unsigned char thirdOutputVoltage;//第3路功放输出电压 0~255， 对应 0~255V Value = data (V)
    unsigned char fourthOutputVoltage;//第4路功放输出电压 0~255， 对应 0~255V Value = data (V)
    unsigned char backup1;//备用
    unsigned char backup2;//备用
    unsigned char outputVoltageData[100];//信号源输出电 压 0~250， 对应 0~5V 备用 Value = data*0.02 (V)
    unsigned char firstOutputVoltageData[100];//第1路功放输出电压 0~255， 对应 0~255V Value = data (V)
    unsigned char secondOutputVoltageData[100];//第2路功放输出电压  0~255， 对应 0~255V Value = data (V)
    unsigned char thirdOutputVoltageData[100];//第3路功放输出电压 0~255， 对应 0~255V Value = data (V)
    unsigned char fourthOutputVoltageData[100];//第4路功放输出电压 0~255， 对应 0~255V Value = data (V)
    unsigned char Backup3[67];//备用
    unsigned short messageTrailer;//报文尾 0x37AA
}T_MsgReply;




class HMsgRequest
{
public:
    HMsgRequest(const char *data = nullptr, int len = 0);
    const char *toEncodedMsg(){return  (const char *)&m_msg;}
    int encodedMsgSize() {return sizeof(T_MsgRequest);}
    int _maxDataSize(){return 8970;}
    int maxDashboradDataSize(){return 7056;} //  1ms 44.1k/16bit/1ch => 8820bytes; it must be <= dataSize() = 8970
    int maxBoradCastDataSize(){return 8820;}
    char *data(){return (char *)m_msg.voiceData;}
    int dataSize(){return sizeof(m_msg.voiceData);}
    void setDataLen(int len){m_msg.backUp = len;}
    void setData(const char *data, int len);
    void setVolume(int val){
        if(val >=0 && val <= 10)
        {
            m_msg.volumeAdjustment = 0x6600 + val;
        }
    }
    void setPowerMod1(bool flag){
        if(flag) m_msg.stateSetting |= 0x1;
        else m_msg.stateSetting &= ~0x1;
    }
    void setPowerMod2(bool flag){
        if(flag) m_msg.stateSetting |= 0x2;
        else m_msg.stateSetting &= ~0x2;
    }
    void setPowerAmp1(bool flag){
        if(flag) m_msg.stateSetting |= 0x4;
        else m_msg.stateSetting &= ~0x4;
    }
    void setPowerAmp2(bool flag){
        if(flag) m_msg.stateSetting |= 0x8;
        else m_msg.stateSetting &= ~0x8;
    }

private:
    T_MsgRequest m_msg;

};



class HMsgReply
{
public:
    HMsgReply(const char *data, int len);

    int emitter_temp() const { return (m_msg != nullptr ? (m_msg->launchTemperature - 65) : 0);}
    int insulation_resistance()const {return (m_msg != nullptr ? (m_msg->insulationResistance * 40) : 0);}
    int powerMod1()const {return (m_msg != nullptr ? (m_msg->powerModuleState1) : 0);}
    int powerMod2()const {return (m_msg != nullptr ? (m_msg->powerModuleState2) : 0);}
    int powerAmp1()const {return (m_msg != nullptr ? (m_msg->ampModuleStatus1) : 0xF);}
    int powerAmp2()const {return (m_msg != nullptr ? (m_msg->ampModuleStatus2) : 0xF);}
    int avgOutVoltag_amp1()const {return (m_msg != nullptr ? (m_msg->firstOutputVoltage) : 0);}
    int avgOutVoltag_amp2()const {return (m_msg != nullptr ? (m_msg->secondOutputVoltage) : 0);}
    int avgOutVoltag_amp3()const {return (m_msg != nullptr ? (m_msg->thirdOutputVoltage) : 0);}
    int avgOutVoltag_amp4()const {return (m_msg != nullptr ? (m_msg->fourthOutputVoltage) : 0);}

private:
    // Here, the reply message is only used for reading.
    // thus we don't need to allocate memory for it, instead, we just need format it
    T_MsgReply *m_msg;
};
#endif // HMESSAGE_H
