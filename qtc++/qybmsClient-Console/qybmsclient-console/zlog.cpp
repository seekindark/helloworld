/*====================================================================================
* File name: 	 zlog.c
* Description:   qDebug Log are written into logfile or  console.
* Project:       EPT
* Note:
* Created on:    2023-5-12
* Author:        Fred.C
=======================================================================================
 History:
---------------------------------------------------------------------------------------
12-05-2023		Version 1.0.0       FC		Initialized version
-------------------------------------------------------------------------------------*/
#include "zlog.h"
#include <QDir>

static ZLog *m_instance;

ZLog::ZLog()
{
    //configurations
    m_logfileMaxSize = 1 * 1024;
    m_renewLogNameMaxNum = 1024;
    m_renewIdx = 0;

    //create log dir which always locates in log/
    QDir dir("log");
    if (!dir.exists())
    {
         QDir dir;
         dir.mkdir("log");
    }
    //create log file
    QString currentDate = QDateTime::currentDateTime().toString("yyyyMMdd");
    QString logName = "log" + currentDate + ".txt";
    QString logFile = "log/" + logName;
    m_logDate = currentDate;

    m_logFh.setFileName(logFile);
    if (!m_logFh.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        m_logFh.close();
    }

}

ZLog::~ZLog()
{
    m_logFh.close();
}

void ZLog::renew_logFileName()
{
    if(m_logFh.size() >= m_logfileMaxSize)
    {
        //close it
        m_logFh.close();

        //renew the file name
        QString currentDate = QDateTime::currentDateTime().toString("yyyyMMdd");
        if(m_logDate == currentDate)
        {
            while(m_renewIdx <= m_renewLogNameMaxNum)
            {
                m_renewIdx += 1;
                currentDate += "-";
                currentDate += QString::number(m_renewIdx);
                QString logName = "log" + currentDate + ".txt";
                QString logFile = "log/" + logName;
                if(!QFileInfo(logFile).exists())
                {
                    m_logFh.setFileName(logFile);
                    break;
                }
            }
        }
    }
}


void ZLog::logout(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
#ifdef QT_NO_DEBUG_OUTPUT
    (void)(context);
#endif
    //create logfile
    QDir dir("log");
    if (!dir.exists())
    {
         QDir dir;
         dir.mkdir("log");
    }

    //创建log文件
    QString currentDate = QDateTime::currentDateTime().toString("yyyyMMdd-hh");
    QString logName = "log" + currentDate + ".txt";
    QString logFileName = "log/" + logName;


    // 加锁
    m_mutex.lock();

    QByteArray localMsg = msg.toLocal8Bit();

    //信息分类
    QString strMsg("");
    switch(type)
    {
    case QtDebugMsg:
        strMsg = QString("Debug");
        break;
    case QtInfoMsg:
        strMsg = QString("Info");
        break;
    case QtWarningMsg:
        strMsg = QString("Warning");
        break;
    case QtCriticalMsg:
        strMsg = QString("Critical");
        break;
    case QtFatalMsg:
        strMsg = QString("Fatal");
        break;

        default:
            break;
    }


    //Release 版本默认不包含context这些信息:文件名、函数名、行数，需要在.pro项目文件加入以下代码，加入后最好重新构建项目使之生效：
    //DEFINES += QT_MESSAGELOGCONTEXT

    //在.pro文件定义以下的宏，可以屏蔽相应的日志输出
    //DEFINES += QT_NO_WARNING_OUTPUT
    //DEFINES += QT_NO_DEBUG_OUTPUT
    //DEFINES += QT_NO_INFO_OUTPUT


    //文件名、函数名、行数
    //strMsg += QString("Function: %1  File: %2  Line: %3 ").arg(context.function).arg(context.file).arg(context.line);

    // 时间和内容
    QString strDateTime = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    QString strMessage = QString("[%1]%2:%3").arg(strDateTime).arg(strMsg).arg(localMsg.constData());


    //写入文件
    QFile file(logFileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        file.close();
        return ;
    }
    QTextStream stream(&file);
    stream << strMessage << "\r\n";;
    file.flush();
    file.close();

    // 解锁
    m_mutex.unlock();

}
