#ifndef WIDSETTING_H
#define WIDSETTING_H

#include <QObject>
#include <QWidget>
#include <QTabWidget>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include "widsetremote.h"
#include "widsetlocal.h"

class WidSetting : public QWidget
{
    Q_OBJECT
public:
    explicit WidSetting(QWidget *parent = nullptr);
    ~WidSetting();
    void setCurrentIndex(int index){m_tab->setCurrentIndex(index);}
    void showEvent(QShowEvent *event);

signals:
public slots:
    void on_tabBarClicked(int);
private:
    QTabWidget *m_tab;
    WidSetLocal *m_widLocal;
    WidSetRemote *m_widRemote1;
    WidSetRemote *m_widRemote2;


};

#endif // WIDSETTING_H
