#include <QtDebug>
#include "filetablewidgetitem.h"


FileTableWidgetItem::FileTableWidgetItem(const QString &text, int type):QTableWidgetItem(text, type)
{
    m_soundFile = nullptr;
    qDebug() << __FUNCTION__;

    m_group = 0;
    m_repeat = false;
    m_alias = -1;       // means NA, for both Warning Alias and tone Alias
}

FileTableWidgetItem::~FileTableWidgetItem()
{
    delete m_soundFile; m_soundFile = nullptr;
    qDebug() << __FUNCTION__;
}
