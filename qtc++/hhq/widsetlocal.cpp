#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QtDebug>
#include <QMessageBox>
#include "widsetlocal.h"
#include "hudpproc.h"
#include "mylog.h"
#include "hsettings.h"

WidSetLocal::WidSetLocal(QWidget *parent) : QWidget(parent)
{
    qDebug() << __FUNCTION__;

    QLabel *lab_ip = new QLabel("IP", this);
    m_LE_ip = new QLineEdit("", this);
    QLabel *lab_port  = new QLabel("Port", this);
    m_LE_port = new QLineEdit("", this);
    m_bt_apply_local = new QPushButton("Apply", this);

    QGroupBox *groupBox_addr = new QGroupBox("Network Address", this);
    //groupBox_addr->setMaximumHeight(200);
    QFormLayout *flayout = new QFormLayout(groupBox_addr);
    flayout->setSpacing(15);
    flayout->addRow(lab_ip, m_LE_ip);
    flayout->addRow(lab_port, m_LE_port);

    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->addWidget(groupBox_addr, 2);
    vLayout->addWidget(m_bt_apply_local, 1);
    QLabel *dummy_Lab = new QLabel();
    vLayout->addWidget(dummy_Lab, 4);


    //load data from config
    m_LE_ip->setText(g_settings->local_ip());
    m_LE_port->setText(g_settings->local_port());

    connect(m_bt_apply_local, SIGNAL(clicked()), this, SLOT(on_apply_clicked()));
    connect(this, SIGNAL(localAddr_changed()), g_udpProc, SLOT(on_localAddr_changed()));
}

WidSetLocal::~WidSetLocal()
{
    qDebug() << __FUNCTION__;
}

void WidSetLocal::on_apply_clicked()
{
    qDebug() << "WidSetLocal::"<<__FUNCTION__<< "input:ip, port" << m_LE_ip->text() <<"," << m_LE_port->text();
    QHostAddress localAddr(m_LE_ip->text().trimmed());
    if(localAddr.isNull())
    {
        QMessageBox::information(this,
                                 "Local Network Address",
                                 "The input IPv4 Address is invalid !");
        m_LE_ip->setText(g_settings->local_ip());
        return;
    }
    int localPort = m_LE_port->text().toInt();
    if(localPort > 65000 || localPort < 1024)
    {
        QMessageBox::information(this,
                                 "Local Network Address",
                                 "The input port is invalid !\n"
                                 "The valid range is 1024 ~ 65000");
        m_LE_port->setText(g_settings->local_port());
        return;
    }
    //save to config
    g_settings->set_local_ip(localAddr.toString());
    g_settings->set_local_port(localPort);

    g_settings->save();

    emit localAddr_changed();

}
