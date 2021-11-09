#include <QMainWindow>
#include <QWidget>
#include <QDebug>
#include <QDateTime>
#include <QFile>
#include "QTextStream"
#include <QDateTime>
#include <QIODevice>
#include "mylog.h"

MyLog::MyLog(const QString &msg)
{
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    //QString current_date = QString("(%1)").arg(current_date_time);
    //QString message = QString("[%1] %2: %3").arg(current_date_time).arg("m_user_name").arg(msg);
    QString message = QString("[%1] : %3").arg(current_date_time).arg(msg);
    QFile file("hhq.log");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.flush();
    file.close();
}
