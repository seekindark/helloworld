/*====================================================================================
* File name: 	 zlog.h
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
#ifndef _zlog_h_
#define _zlog_h_
#include <QTime>
#include <QMutex>
#include <QFile>

class ZLog
{
public:
    ZLog();
    ~ZLog();

    static ZLog* get_handler();

    void logout(QtMsgType type, const QMessageLogContext &context, const QString &msg);

private:
    QMutex m_mutext;
    QString m_logDate;
    QFile   m_logFh;
    uint32_t m_logfileMaxSize;
    uint32_t m_renewLogNameMaxNum;
    uint32_t m_renewIdx;
    

    void renew_logFileName();
};



#endif
