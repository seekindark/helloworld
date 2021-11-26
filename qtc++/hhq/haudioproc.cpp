#include <QDebug>
#include <QMessageBox>
#include "haudioproc.h"
#include "mylog.h"
#include "filetablewidgetitem.h"
#include "hudpproc.h"
#include "hmessage.h"
#include "hsettings.h"

HAudioProc::HAudioProc(QObject *parent):QObject(parent),
    m_audioFormat()
{
    qDebug() << __FUNCTION__;

    m_audioInput = nullptr;
    m_audioOutput = nullptr;
    m_is_able = true;
    //audio buffer
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

    // play-in/out progress
    m_pb_out = nullptr;
    m_pb_in = nullptr;
    //
    // for page: new recording
    //
    m_bt_audio = nullptr;

    //
    // for edit recording
    //
    m_bt_editReview = nullptr;

    //
    // for dashboard
    //
    m_bt_play2remote = nullptr;
    connect(&m_sndTimer, SIGNAL(timeout()), this, SLOT(on_sndTimerout()));
    //
    // for broadcast
    //
    m_bt_broadcast = nullptr;
    m_broadcastBufIntervals = 1;
    m_read_pos = 0;
}

HAudioProc::~HAudioProc()
{
    qDebug() << __FUNCTION__;

    delete m_audioInput;
    delete m_audioOutput;

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
        m_pb_in = nullptr;
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
    m_voiceData.clear();

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
    m_bt_audio = nullptr;
    m_pb_in = nullptr;
    m_pb_out = nullptr;

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

    if(m_bt_broadcast != nullptr && m_broadcastBufIntervals > 0)
    {
        m_broadcastBufIntervals -= 1;
        if(m_broadcastBufIntervals == 0)
        {
            //start a timer for sending voice periodically
            m_sndTimer.start(70);

            broadcast_play();
        }
    }

}
void HAudioProc::on_notify_output()
{
/*
    qDebug() <<__FUNCTION__
         << "\nbuffersize, bytesFree, periodSize, processedUSecs, elapsedUSecs, bufDevSize, voiceSize, pos\n"
          << m_audioOutput->bufferSize()
          << m_audioOutput->bytesFree()
          << m_audioOutput->periodSize()
          << m_audioOutput->processedUSecs()
          << m_audioOutput->elapsedUSecs()
          << m_bufDevice.size()
          << m_voiceData.size()
          << m_bufDevice.pos();
*/
    if(m_pb_out != nullptr)
    {
        //it's played over once, check if it's stopped or repeated again.
        if(m_pb_out->value() == int(m_bufDevice.size()))
        {
            // page: recording reviw
            if(m_bt_audio != nullptr)
            {
                //if it's played over, then stop it by clicking the button
                m_bt_audio->click();
            }
            //page: edit review
            if(m_bt_editReview !=nullptr)
            {
                //review done, stop it
                m_bt_editReview->stop();
            }
        }
        else
        {
            // continue to play
            m_pb_out->setValue(int(m_bufDevice.pos()));
        }
    }

}

void HAudioProc::on_sndTimerout()
{
    qDebug(__FUNCTION__);

    //page: dashboard
    if(m_bt_play2remote != nullptr)
    {
        //it's played over once, check if it's stopped or repeated again.
        if(m_pb_out->value() == int(m_bufDevice.size()))
        {
            const FileTableWidgetItem * fitem = m_bt_play2remote->fileWidItem_r();
            if(fitem->isRepeat())
            {
                //reset to the beginning and play it once again
                qDebug() <<__FUNCTION__ << "palyed2remote once again !";

                //replay it
                m_bufDevice.seek(0);
                m_pb_out->setValue(0);

                play2remote();
            }
            else
            {
                //stop it
                m_bt_play2remote->click();
            }
        }
        else
        {
            m_pb_out->setValue(int(m_bufDevice.pos()));

            //continue to send out voice data
            //when it's playing to remote, the audiooutput is NOT in "state" mode, thus it can not send out automatically
            play2remote();
        }

    }
    //page: broadcast
    else if(m_bt_broadcast != nullptr)
    {
        broadcast_play();
    }
}

//interfaces for 'EditRecord' page
HAudioProc::E_PlayOutResult
HAudioProc::ed_review_start(ButtonEdit * btEdit, QProgressBar *pb_out)
{
    qDebug(__FUNCTION__);
    Q_ASSERT(btEdit != nullptr);

    //not the same one and the previous button is playing then stop it
    if(m_bt_editReview != nullptr && m_bt_editReview != btEdit)
    {
        m_bt_editReview->stop();

    }
    m_bt_editReview = btEdit;
    m_pb_out = pb_out;

    FileTableWidgetItem * fitem_wr = btEdit->fileWidItem_wr();
    Q_ASSERT(fitem_wr != nullptr);

    QFile *sf = fitem_wr->soundFile();
    if(sf == nullptr)
    {
        qDebug() <<__FUNCTION__ <<"not filename given!";
        m_bt_editReview = nullptr;
        m_pb_out = nullptr;
        return e_playout_noFileName;
    }
    if(!sf->exists())
    {
        qDebug() <<__FUNCTION__ <<sf->fileName() << "not exists!";
        m_bt_editReview = nullptr;
        m_pb_out = nullptr;
        return e_playout_fileNotExist;
    }

    bool is_wavFormat = is_default_wav_file(sf);
    sf->open(QIODevice::ReadOnly);
    if(is_wavFormat)
    {
        sf->seek(wave_head_size());
    }
    m_bufDevice.open(QIODevice::ReadWrite);
    m_voiceData.clear();
    m_bufDevice.reset();
    m_bufDevice.write(sf->readAll());
    sf->close();


    m_bufDevice.seek(0);
    m_pb_out->setRange(0, int(m_bufDevice.size()));
    m_pb_out->setValue(0);
    m_audioOutput->start(&m_bufDevice);

    qDebug() << __FUNCTION__ << (is_wavFormat?"WAV":"PCM")
             <<"File, sf.size =" <<sf->size()
             << "start from pos =" << sf->pos()
             << "bufDevice.size=" << m_bufDevice.size()
             << "bufDevce.availBytes=" << m_bufDevice.bytesAvailable();

    return e_playout_OK;
}
void HAudioProc::ed_review_stop()
{
    qDebug(__FUNCTION__);

    m_audioOutput->stop();
    m_bufDevice.close();
    m_bt_editReview = nullptr;
    m_pb_out = nullptr;
}


bool HAudioProc::is_default_wav_file(QFile *sf)
{
    Q_ASSERT(sf != nullptr);
    if(sf->size() < wave_head_size())
    {
        return false;
    }
    else
    {
        T_wavHeadFmt head;
        memset(&head, 0, sizeof(head));
        sf->open(QIODevice::ReadOnly);
        sf->read((char *)&head, sizeof(head));

        qDebug() <<__FUNCTION__
                <<"head.RiffName, WavName, FmtName, DataName, nAudioFormat, nSampleRate"
                <<QByteArray(head.RiffName, 4)
                <<QByteArray(head.WavName, 4)
                <<QByteArray(head.FmtName, 3)
                <<QByteArray(head.DATANAME, 4)
                <<head.nAudioFormat
                <<head.nSampleRate;

        if(!qstrnicmp(head.RiffName,"RIFF",4) &&
           !qstrncmp(head.WavName,"WAVE", 4) &&
           !qstrncmp(head.FmtName,"fmt", 3) &&
           !qstrncmp(head.DATANAME,"data", 4) &&
           head.nAudioFormat == 1 /*PCM*/)
        {
            sf->close();
            return true;
        }

        sf->close();
    }

    return false;
}

HAudioProc::E_PlayOutResult
HAudioProc::play2remote_start(BtnPlay *bt_play2remote, QProgressBar *pb_out)
{
    qDebug(__FUNCTION__);
    Q_ASSERT(bt_play2remote != nullptr);
    Q_ASSERT(pb_out != nullptr);

    //not the same one and the previous button is playing then stop it
    if(m_bt_play2remote != nullptr && m_bt_play2remote != bt_play2remote)
    {
        m_bt_play2remote->on_clicked();

    }
    //udpate for the current playing
    m_bt_play2remote = bt_play2remote;
    m_pb_out = pb_out;

    const FileTableWidgetItem * fitem = bt_play2remote->fileWidItem_r();
    if(fitem != nullptr)
    {
        QFile *sf = fitem->soundFile();
        if(sf == nullptr)
        {
            m_bt_play2remote = nullptr;
            m_pb_out = nullptr;
            qDebug() <<__FUNCTION__ <<"not filename given!";
            return e_playout_noFileName;
        }
        if(!sf->exists())
        {
            m_bt_play2remote = nullptr;
            m_pb_out = nullptr;
            qDebug() <<__FUNCTION__ <<sf->fileName() << "not exists!";
            return e_playout_fileNotExist;
        }

        bool is_wavFormat = is_default_wav_file(sf);
        sf->open(QIODevice::ReadOnly);
        if(is_wavFormat)
        {
            sf->seek(wave_head_size());
        }
        m_bufDevice.open(QIODevice::ReadWrite);
        m_voiceData.clear();
        m_bufDevice.reset();
        m_bufDevice.write(sf->readAll());
        sf->close();


        m_bufDevice.seek(0);
        m_pb_out->setRange(0, int(m_bufDevice.size()));
        m_pb_out->setValue(0);

        //start a timer and send out first one
        m_sndTimer.start(70);
        play2remote();

        qDebug() << __FUNCTION__ << (is_wavFormat?"WAV":"PCM")
                 <<"File, sf.size =" <<sf->size()
                 << "start from pos =" << sf->pos()
                 << "bufDevice.size=" << m_bufDevice.size()
                 << "bufDevce.availBytes=" << m_bufDevice.bytesAvailable();
    }
    else
    {
        qDebug() <<__FUNCTION__ <<"No sound file set here!";
        m_bt_play2remote = nullptr;
        m_pb_out = nullptr;
        return e_playout_noFileName;
    }

    return e_playout_OK;
}
void HAudioProc::play2remote_stop()
{
    qDebug() << __FUNCTION__;
    m_audioOutput->stop();
    m_sndTimer.stop();  // the timer must be stopped here!!
    m_bufDevice.close();
    m_voiceData.clear();
    m_bt_play2remote = nullptr;
    m_pb_out = nullptr;
}

void HAudioProc::play2remote()
{
    HMsgRequest msg;
    int len = m_bufDevice.read(msg.data(), msg.maxDashboradDataSize());
    msg.setDataLen(len);
    g_udpProc->sendHMsgReq(msg);

    qDebug() << __FUNCTION__ << "send out msgSize=" << len;
}

void HAudioProc::broadcast_start(QPushButton *bt_mic, QProgressBar *pb_in, QProgressBar *pb_out)
{
    qDebug() <<__FUNCTION__;

    m_bufDevice.open( QIODevice::WriteOnly | QIODevice::Truncate);
    m_pb_in = pb_in;
    m_pb_out = pb_out;
    m_bt_broadcast = bt_mic;

    //for broadcasting, pb_in is alwasy in 1~10, i.e. max is 1 sec
    m_pb_in->setRange(0,10);
    m_pb_in->setValue(0);
    // pb_out is also expressed as 1 sec in bytes unit, the max is 8820*10 i.e. 1 sec sampling bits
    m_pb_out->setRange(0, 88200);
    m_pb_out->setValue(0);

    //start recording and collect audio bits
    m_audioInput->start(&m_bufDevice);
    m_broadcastBufIntervals = 3;
    m_read_pos = 0;


}
void HAudioProc::broadcast_stop()
{
    qDebug() <<__FUNCTION__;

    m_audioInput->stop();
    m_audioOutput->stop();
    m_sndTimer.stop();
    m_bufDevice.close();
    m_voiceData.clear();

    m_pb_in->setValue(0);
    m_pb_out->setValue(0);
    m_pb_in = nullptr;
    m_pb_out = nullptr;
    m_bt_broadcast = nullptr;

}
void HAudioProc::broadcast_play()
{
    HMsgRequest msg;
    qint64 in_pos = m_bufDevice.pos();
    qint64 available_bytes = in_pos - m_read_pos;
    if(available_bytes > 0)
    {
        int voice_data_len = msg.maxBoradCastDataSize();
        if(available_bytes <= voice_data_len)
        {
            voice_data_len = int(available_bytes);
        }
        msg.setData(m_bufDevice.data().data() + m_read_pos, voice_data_len);
        m_read_pos += voice_data_len;   // move read_pos !!

        g_udpProc->sendHMsgReq(msg);

        //update pb-out
        int val = m_pb_out->value() + voice_data_len;
        if(val > m_pb_out->maximum())
            val -= m_pb_out->maximum();
        m_pb_out->setValue(val);

        qDebug() << __FUNCTION__ << "send out msgSize=" << voice_data_len << "left bytes=" << available_bytes - voice_data_len;
    }
    else
    {
        qDebug() << __FUNCTION__ << "!!===No data to send===, available_bytes = " << available_bytes;
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
        devBuf.close();
        return -1;
    }

    fd.write((const char *)&wavhead,sizeof(T_wavHeadFmt));
    fd.write(devBuf.readAll());

    fd.close();
    devBuf.close();

    return (wavhead.nRiffLength + 8);
}

void HAudioProc::reset()
{
    m_audioInput->stop();
    m_audioOutput->stop();
    m_bufDevice.close();
    m_voiceData.clear();

    if(m_bt_editReview != nullptr)
    {
        m_bt_editReview->stop();
    }

    if(m_bt_play2remote != nullptr)
    {
        m_bt_play2remote->click();
    }

    if(m_bt_broadcast != nullptr)
    {
        m_bt_broadcast->click();
    }
}
