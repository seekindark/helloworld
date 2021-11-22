#include <QDebug>
#include "haudioproc.h"
#include "mylog.h"

HAudioProc::HAudioProc(QObject *parent):QObject(parent),
    m_raw(),
    m_audioFormat()
{
    qDebug() << __FUNCTION__;

    m_audioInput = nullptr;
    m_audioOutput = nullptr;
    m_file = nullptr;
    m_is_able = true;
    m_pb_in = nullptr;
    m_pb_out = nullptr;

    //audio buffer
    m_raw.setFileName("buf.raw");
    m_bufDevice.setBuffer(&m_voiceData);

    //default audio format. only support this one !
    m_audioFormat.setByteOrder(QAudioFormat::LittleEndian);
    m_audioFormat.setChannelCount(1);
    m_audioFormat.setCodec("audio/pcm");
    m_audioFormat.setSampleRate(44100);     //44.1K is fixed as requirement
    m_audioFormat.setSampleSize(16);
    m_audioFormat.setSampleType(QAudioFormat::UnSignedInt);

    //check if the MC device is existing
    QAudioDeviceInfo devInfo = QAudioDeviceInfo::defaultInputDevice();
    if(devInfo.isNull())
    {
        qDebug() << "MC device NOT FOUND !! No way to do recording and play !";
        MyLog("MC device NOT FOUND !! No way to do recording and play !");
        m_is_able = false;
        return;
    }

    //check if current device supports the audio format
    if(!devInfo.isFormatSupported(m_audioFormat))
    {
        //if it's not, we can try to conver to the nearest format
        m_audioFormat = devInfo.nearestFormat(m_audioFormat);
        qDebug() << __FUNCTION__ << "default audio format is changed to nearest format:"
                 << m_audioFormat;
    }
    else
    {
         qDebug() << __FUNCTION__ <<"Default Audio Format:" << m_audioFormat;
    }

    m_audioInput = new QAudioInput(devInfo, m_audioFormat, this);
    m_audioInput->setNotifyInterval(100);
    connect(m_audioInput, SIGNAL(notify()), this, SLOT(on_notify_input()));

    m_audioOutput = new QAudioOutput(devInfo, m_audioFormat, this);
    m_audioOutput->setNotifyInterval(100);
    connect(m_audioOutput, SIGNAL(notify()), this, SLOT(on_notify_output()));

    //timer
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(on_timeout()));
}

HAudioProc::~HAudioProc()
{
    qDebug() << __FUNCTION__;

    delete m_audioInput;
    delete m_audioOutput;
    delete m_file;

}

void HAudioProc::recording_start(QProgressBar *pb_in, QPushButton *bt_audio)
{
    qDebug() << __FUNCTION__ << "is_able = " <<is_able();
    if(! is_able()) return;

    m_bufDevice.open( QIODevice::WriteOnly | QIODevice::Truncate);
    m_pb_in = pb_in;
    m_bt_audio = bt_audio;
    m_pb_in->setRange(0,10);
    m_pb_in->setValue(0);
    m_audioInput->start(&m_bufDevice);
}
void HAudioProc::recording_stop()
{
    //qDebug() << __FUNCTION__;

    if(! is_able()) return;

    m_audioInput->stop();
    m_bufDevice.close();

    if(m_pb_in != nullptr)
    {
        m_pb_in->setValue(0);
    }

    qDebug() <<__FUNCTION__
             << "\nbuffersize, bytesReady, periodSize, processedUSecs, elapsedUSecs, bufDevSize, voiceSize, pos\n"
              << m_audioInput->bufferSize()
              << m_audioInput->bytesReady()
              << m_audioInput->periodSize()
              << m_audioInput->processedUSecs()
              << m_audioInput->elapsedUSecs()
              << m_bufDevice.size()
              << m_voiceData.size()
              << m_bufDevice.pos();

}

void HAudioProc::recording_discard()
{
    qDebug() << __FUNCTION__;
    if(! is_able()) return;


    m_audioOutput->stop(); // stop anyway if it's discarded
    m_audioInput->reset();
    m_bufDevice.reset();

    qDebug() <<__FUNCTION__
             << "\nbuffersize, bytesReady, periodSize, processedUSecs, elapsedUSecs, bufDevSize, voiceSize, pos\n"
              << m_audioInput->bufferSize()
              << m_audioInput->bytesReady()
              << m_audioInput->periodSize()
              << m_audioInput->processedUSecs()
              << m_audioInput->elapsedUSecs()
              << m_bufDevice.size()
              << m_voiceData.size()
              << m_bufDevice.pos();
}


qint64 HAudioProc::recording_save(const QString &filename)
{
    qDebug() << __FUNCTION__ << filename;
    qDebug() <<__FUNCTION__
             << "\nbuffersize, bytesReady, periodSize, processedUSecs, elapsedUSecs, bufDevSize, voiceSize, pos\n"
              << m_audioInput->bufferSize()
              << m_audioInput->bytesReady()
              << m_audioInput->periodSize()
              << m_audioInput->processedUSecs()
              << m_audioInput->elapsedUSecs()
              << m_bufDevice.size()
              << m_voiceData.size()
              << m_bufDevice.pos();

    m_audioOutput->stop(); // stop anyway if it's discarded

    //start to write the buffer into a .wav file
    //
    return write_into_wav(m_bufDevice, filename);

}
void HAudioProc::review_start(QProgressBar * pb_out, QPushButton *bt_audio)
{
    qDebug() <<__FUNCTION__
             << "\nbuffersize, bytesReady, periodSize, processedUSecs, elapsedUSecs, bufDevSize, voiceSize, pos\n"
              << m_audioInput->bufferSize()
              << m_audioInput->bytesReady()
              << m_audioInput->periodSize()
              << m_audioInput->processedUSecs()
              << m_audioInput->elapsedUSecs()
              << m_bufDevice.size()
              << m_voiceData.size()
              << m_bufDevice.pos();
    if(!is_able()) return;

    m_pb_out = pb_out;
    m_bt_audio = bt_audio;
    m_pb_out->setRange(0, int(m_bufDevice.size()));
    m_pb_out->setValue(0);
    m_bufDevice.open( QIODevice::ReadOnly);
    m_audioOutput->start(&m_bufDevice);

}
void HAudioProc::review_stop()
{
    //qDebug() << __FUNCTION__ << "is_able = " << is_able();
    if(! is_able()) return;

    qDebug() <<__FUNCTION__
             << "\nbuffersize, bytesReady, periodSize, processedUSecs, elapsedUSecs, bufDevSize, voiceSize, pos\n"
              << m_audioInput->bufferSize()
              << m_audioInput->bytesReady()
              << m_audioInput->periodSize()
              << m_audioInput->processedUSecs()
              << m_audioInput->elapsedUSecs()
              << m_bufDevice.size()
              << m_voiceData.size()
              << m_bufDevice.pos();

    m_audioOutput->stop();
    m_bufDevice.close();

    if(m_pb_out != nullptr )
    {
        m_pb_out->setValue(0);
    }

}

void HAudioProc::on_timeout()
{
    qDebug() <<__FUNCTION__
             << ": \nbuffersize, bytesReady, periodSize, processedUSecs, elapsedUSecs, bufDevSize, voiceSize, pos\n"
              << m_audioInput->bufferSize()
              << m_audioInput->bytesReady()
              << m_audioInput->periodSize()
              << m_audioInput->processedUSecs()
              << m_audioInput->elapsedUSecs()
              << m_bufDevice.size()
              << m_voiceData.size()
              << m_bufDevice.pos();
}
void HAudioProc::on_notify_input()
{
    /*
    qDebug() <<__FUNCTION__
         << " : \nbuffersize, bytesReady, periodSize, processedUSecs, elapsedUSecs, bufDevSize, voiceSize, pos\n"
          << m_audioInput->bufferSize()
          << m_audioInput->bytesReady()
          << m_audioInput->periodSize()
          << m_audioInput->processedUSecs()
          << m_audioInput->elapsedUSecs()
          << m_bufDevice.size()
          << m_voiceData.size()
          << m_bufDevice.pos();
*/
    if(m_pb_in != nullptr)
    {
        int val = m_pb_in->value() + 1;
        if(val > 10) val = 1;
        m_pb_in->setValue(val);
    }

}
void HAudioProc::on_notify_output()
{
/*
    qDebug() <<__FUNCTION__
         << "\nbuffersize, bytesReady, periodSize, processedUSecs, elapsedUSecs, bufDevSize, voiceSize, pos\n"
          << m_audioInput->bufferSize()
          << m_audioInput->bytesReady()
          << m_audioInput->periodSize()
          << m_audioInput->processedUSecs()
          << m_audioInput->elapsedUSecs()
          << m_bufDevice.size()
          << m_voiceData.size()
          << m_bufDevice.pos();
*/
    if(m_pb_out != nullptr)
    {
        if(m_pb_out->value() == int(m_bufDevice.size()))
        {
            if(m_bt_audio != nullptr)
            {
                //if it's played over, then stop it by clicking the button
                m_bt_audio->click();
            }
        }
        else
        {
            m_pb_out->setValue(int(m_bufDevice.pos()));
        }

    }
}

qint64 HAudioProc::write_into_wav(QBuffer &devBuf, const QString &filename)
{
    qDebug(__FUNCTION__);

    T_wavHeadFmt wavhead;
    qstrcpy(wavhead.RiffName,"RIFF");
    qstrcpy(wavhead.WavName, "WAVE");
    qstrcpy(wavhead.FmtName, "fmt ");
    qstrcpy(wavhead.DATANAME,"data");

    wavhead.nFmtLength = 16;
    wavhead.nAudioFormat = 1; //pcm
    wavhead.nChannleNumber = 1;
    wavhead.nSampleRate = 44100;
    // nBytesPerSample 和 nBytesPerSecond这两个值通过设置的参数计算得到;
    // 数据块对齐单位(每个采样需要的字节数 = 通道数 × 每次采样得到的样本数据位数 / 8 )
    wavhead.nBytesPerSample = 2;
    // 波形数据传输速率
    // (每秒平均字节数 = 采样频率 × 通道数 × 每次采样得到的样本数据位数 / 8  = 采样频率 × 每个采样需要的字节数 )
    wavhead.nBytesPerSecond = 88200;
    // 每次采样得到的样本数据位数;
    wavhead.nBitsPerSample = 16;
    //lenth
    wavhead.nRiffLength = devBuf.size() + sizeof(T_wavHeadFmt) - 8;

    //start to write wave file
    if(!devBuf.open(QIODevice::ReadOnly))
    {
        qDebug() << __func__ <<"Failed to open devBuf";
        return -1;
    }
    QFile fd(filename);
    if( !fd.open(QIODevice::WriteOnly))
    {
        qDebug() << __func__ <<"Failed to open filename=" << filename;
        return -1;
    }

    fd.write((const char *)&wavhead,sizeof(T_wavHeadFmt));
    fd.write(devBuf.readAll());

    fd.close();
    devBuf.close();

    return (wavhead.nRiffLength + 8);
}
