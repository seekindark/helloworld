#include <QDebug>
#include "hudpproc.h"
#include "hsettings.h"


HUdpProc::HUdpProc(QObject *parent) : QObject(parent)
{
    qDebug(__FUNCTION__);
}




void HUdpProc::on_localAddr_changed()
{
    qDebug(__FUNCTION__);

}


void HUdpProc::on_remoteChanged(int remoteId)
{
    qDebug() << __FUNCTION__ << "ID=" << remoteId;
}
