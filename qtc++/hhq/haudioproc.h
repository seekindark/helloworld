#ifndef HAUDIOPROC_H
#define HAUDIOPROC_H

#include <QObject>
#include <QByteArray>
#include <QDateTime>
#include <QAudioRecorder>
#include <QAudioProbe>
#include <QTimer>
#include <QMessageBox>
#include <QAudioFormat>
#include <QFile>
#include <QAudioInput>
#include <QAudioOutput>
#include <QBuffer>
#include <QProgressBar>
#include <QPushButton>


class HAudioProc:public QObject
{
    Q_OBJECT
public:
    explicit HAudioProc(QObject *parent = nullptr);
    ~HAudioProc();

    typedef struct
    {
        // RIFF head
        char RiffName[4];// 'RIFF'
        unsigned long nRiffLength;  //FILESIZE - 8
        // FILE TYPE
        char WavName[4];    //'WAVE'
        // Format block
        char FmtName[4];    // 'fmt'
        unsigned long nFmtLength;   //16
        unsigned short nAudioFormat;        //audio/pcm=1
        unsigned short nChannleNumber;      //1
        unsigned long nSampleRate;          //44.1k
        unsigned long nBytesPerSecond;      //88.2k
        unsigned short nBytesPerSample;     //2
        unsigned short nBitsPerSample;      //16
        // block head
        char    DATANAME[4];            //'data'
        unsigned long   nDataLength;    //ByteRate * seconds
    }T_wavHeadFmt;



    void recording_start(QProgressBar *pb_in, QPushButton *bt_audio);
    void recording_stop();
    void recording_discard();
    qint64 recording_save(const QString &filename);
    void review_start(QProgressBar *pb_out, QPushButton *bt_audio);
    void review_stop();
    bool is_able(){return m_is_able;}


public slots:
    void on_notify_input();
    void on_notify_output();
    void on_timeout();

signals:


private:
    qint64 write_into_wav(QBuffer &devBuf, const QString &filename);


    QAudioInput *m_audioInput;
    QAudioOutput *m_audioOutput;

    QFile *m_file;
    QPushButton *m_bt_audio;
    QProgressBar *m_pb_in;
    QProgressBar *m_pb_out;

    //File-style buffer
    QFile m_raw;
    // byte-arrary buffer, quicker than file-style
    QByteArray      m_voiceData;
    QBuffer         m_bufDevice;
    qint64          m_read_pos;

    QTimer         m_timer;

    //temp buffer to store the recording before it's saved to a file provided by user
    bool m_is_able;

    QAudioFormat m_audioFormat;
};

#endif // HAUDIOPROC_H
