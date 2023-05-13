#ifndef VERSION_H
#define VERSION_H
#include <QTime>

#define BUILD_DATE      (QDate::currentDate().toString("yyyy-MM-dd"))
#define BUILD_TIME      (QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))
#define BUILD_VERSION_Major   1     //1~255
#define BUILD_VERSION_Minor   6     //0~255
#define BUILD_VERSION_Patch   1     //0~65535


#endif // VERSION_H
