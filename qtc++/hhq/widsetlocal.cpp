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

    QGridLayout *gridLay = new QGridLayout(this);
    //gridLay->setContentsMargins(10, 10, 10, 10);
    gridLay->addWidget(groupBox_addr, 0, 0);
    gridLay->addWidget(m_bt_apply_local, 1, 0);

    //status side
    m_LE_sockState = new QLineEdit("", this);
    m_LE_packetRx = new QLineEdit("", this);
    m_LE_packetTx = new QLineEdit("", this);
    m_LE_byteRx = new QLineEdit("", this);
    m_LE_byteTx = new QLineEdit("", this);
    QGroupBox *groupBox_netStatus = new QGroupBox("Network Status", this);
    groupBox_netStatus->setDisabled(true);
    //groupBox_addr->setMaximumHeight(200);
    QFormLayout *flayout_ns = new QFormLayout(groupBox_netStatus);
    flayout_ns->setSpacing(15);
    flayout_ns->addRow("Socket Status:", m_LE_sockState);
    flayout_ns->addRow("Packets Received:", m_LE_packetRx);
    flayout_ns->addRow("Packets Sent:", m_LE_packetTx);
    flayout_ns->addRow("Bytes Received:", m_LE_byteRx);
    flayout_ns->addRow("Bytes Sent:", m_LE_byteTx);

    gridLay->addWidget(groupBox_netStatus, 0, 1);
    gridLay->setRowStretch(0, 2);
    gridLay->setRowStretch(1, 1);

    //load data from config
    load_config();

    /*
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->addWidget(groupBox_addr, 2);
    vLayout->addWidget(m_bt_apply_local, 1);
    QLabel *dummy_Lab = new QLabel();
    vLayout->addWidget(dummy_Lab, 4);
    */

    connect(m_bt_apply_local, SIGNAL(clicked()), this, SLOT(on_apply_clicked()));
    connect(this, SIGNAL(localAddr_changed()), g_udpProc, SLOT(on_localAddr_changed()));


    //start a timer to update the  status
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL (timeout()), this, SLOT(on_statusTimerout()));
    m_timer->start(500);
}

void WidSetLocal::load_config()
{

    m_LE_ip->setText(g_settings->local_ip());
    m_LE_port->setText(g_settings->local_port());
}
WidSetLocal::~WidSetLocal()
{
    qDebug() << __FUNCTION__;
}

void WidSetLocal::on_statusTimerout()
{
    //qDebug() << "WidSetLocal::" << __FUNCTION__;
    m_LE_sockState->setText(g_udpProc->socketStatus());
    m_LE_packetRx->setText(QString::number(g_udpProc->packetRx()));
    m_LE_packetTx->setText(QString::number(g_udpProc->packetTx()));
    m_LE_byteRx->setText(QString::number(g_udpProc->bytesRx()));
    m_LE_byteTx->setText(QString::number(g_udpProc->bytesTx()));
}

void WidSetLocal::on_apply_clicked()
{
    qDebug() << "WidSetLocal::"<<__FUNCTION__<< "input:ip, port" << m_LE_ip->text() <<"," << m_LE_port->text();

    /*
     *   Note: ipStr or PortStr is empty means this remote is disabled.
    */

    QString ipStr = m_LE_ip->text().trimmed();
    QHostAddress localAddr(ipStr);

    if(localAddr.isNull())
    {
        QMessageBox::information(this,
                                 "Local Network Address",
                                 "The input IPv4 Address is invalid !");
        m_LE_ip->setText(g_settings->local_ip());
        return;
    }
    QString portStr = m_LE_port->text().trimmed();
    int localPort = m_LE_port->text().toInt();
    if((localPort > 65000 || localPort < 1024))
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
