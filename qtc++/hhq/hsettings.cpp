#include <QDebug>
#include "hsettings.h"

HSettings::HSettings(QObject *parent) : QObject(parent)
{
    qDebug() << __FUNCTION__;

    m_cfg_filename = "config/config.ini";
    m_settings = new QSettings(m_cfg_filename, QSettings::IniFormat);

    display();


}

void HSettings::display()
{
    qDebug() << "HSettings::" << __FUNCTION__;

    qDebug() << m_settings->allKeys();
    qDebug() << m_settings->childGroups();
    qDebug() << m_settings->childKeys();

    qDebug() << m_settings->value("Local/ip", "192.168.30.1").toString();
    qDebug() << m_settings->value("Local/port", 20481).toUInt();
    qDebug() << m_settings->value("Remote-1/module-enablement", "192.168.30.115").toString();
    qDebug() << m_settings->value("Remote-2/module-enablement", "auto").toString();

}
