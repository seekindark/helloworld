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
#include "buttonedit.h"
#include "btnplay.h"


class HAudioProc:public QObject
{
    Q_OBJECT
public:
    explicit HAudioProc(QObject *parent = nullptr);
    ~HAudioProc();

    typedef enum
    {
        e_playout_OK = 0,
        e_playout_noFileName = -1,
        e_playout_fileNotExist = -2,


    }E_PlayOutResult;

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

    void reset();
    bool is_able(){return m_is_able;}

    //interfaces for 'New Recording' page
    void recording_start(QProgressBar *pb_in, QPushButton *bt_audio);
    void recording_stop();
    void recording_discard();
    qint64 recording_save(const QString &filename);
    void review_start(QProgressBar *pb_out, QPushButton *bt_audio);
    void review_stop();

    //interfaces for 'EditRecord' page
    E_PlayOutResult ed_review_start(ButtonEdit * btEdit, QProgressBar *pb_out);
    void ed_review_stop();

    //intefaces for "dashboard" page
    E_PlayOutResult play2remote_start(BtnPlay *bt_play2remote, QProgressBar *pb_out);
    void play2remote_stop();

    //intefaces for "broadcast" page
    void broadcast_start(QPushButton *bt_mic, QProgressBar *pb_in, QProgressBar *pb_out);
    void broadcast_stop();


public slots:
    void on_notify_input();
    void on_notify_output();


signals:


private:
    int wave_head_size(){return sizeof(T_wavHeadFmt);}
    bool is_default_wav_file(QFile *sf);
    qint64 write_into_wav(QBuffer &devBuf, const QString &filename);

    void play2remote();
    //
    //common for all pages
    //
    //temp buffer to store the recording before it's saved to a file provided by user
    bool m_is_able;
    QAudioFormat m_audioFormat;
    QAudioInput *m_audioInput;
    QAudioOutput *m_audioOutput;
    // byte-arrary buffer, quicker than file-style
    QByteArray      m_voiceData;
    QBuffer         m_bufDevice;

    qint64          m_read_pos;
    QProgressBar *  m_pb_out;       //out play progress
    QProgressBar *  m_pb_in;        //input progress,

    //
    //page: New recording
    //
    QPushButton *m_bt_audio;



    //
    //page: Edit recording
    //
    ButtonEdit *m_bt_editReview;
    //
    // Page: Dashboard
    //
    BtnPlay *m_bt_play2remote;
    //
    //page: broadcast
    //
    QPushButton *m_bt_broadcast;

};


extern HAudioProc *g_audioProc;

#endif // HAUDIOPROC_H
