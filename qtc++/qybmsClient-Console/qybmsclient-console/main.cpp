#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>
#include <QDebug>
#include <iostream>
#include "can_tp.h"
#include "canthread.h"
#include "testcantp.h"

#define	CANID_SWUP_TEST         0X6FD
#define CANID_SWUP_TPTX         (CANID_SWUP_TEST)
#define CANID_SWUP_TPRX         (CANID_SWUP_TEST + 1)


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "qybmsclient-console_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    CANThread *m_canthread = new CANThread();
    CAN_TP *m_canTpLink = new CAN_TP(m_canthread, CANID_SWUP_TPTX, CANID_SWUP_TPRX);
    m_canthread->register_canTP(m_canTpLink);

    m_canthread->set_params(VCI_USBCAN2,    //device type
                            0,              //device index
                            0,              //can channel index
                          250,              //kbps
                          CANThread::e_filter_swup_only);

    //Open Device
    if(m_canthread->openDevice())
    {
        qDebug("openDevice OK");
        if(m_canthread->initCAN())
        {
            qDebug("initCAN OK");
            if(m_canthread->readBoardInfo())
            {
                //do nothing
            }
            else
            {
                qDebug("readBoardInfo Failed");
                return -1;
            }
        }
        else
        {
            qDebug("InitCan Failed");
            return -1;
        }
    }
    else
    {
        qDebug("OpenDevice Failed");
        return -1;
    }

    //connect
    if(m_canthread->startCAN())
    {
        qDebug("startCAN OK");
        //start thread to receive can messages
        m_canthread->start_run(QThread::HighestPriority);
        m_canTpLink->start_run(QThread::HighestPriority);
    }
    else
    {
       qDebug("StartCan Failed");
       return -1;
    }

    testCanTp *testcantp_app = new testCanTp(m_canthread, m_canTpLink);
    QObject::connect(testcantp_app,SIGNAL(finished()),&a,SLOT(quit()));
    testcantp_app->start_run();

    return a.exec();
}
