#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QtDebug>
#include "widsetlocal.h"

WidSetLocal::WidSetLocal(QWidget *parent) : QWidget(parent)
{
    qDebug() << __FUNCTION__;

    QLabel *lab_ip = new QLabel("IP", this);
    m_LE_ip = new QLineEdit("192.168.30.1", this);
    QLabel *lab_port  = new QLabel("Port", this);
    m_LE_port = new QLineEdit("20481", this);
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

    connect(m_bt_apply_local, SIGNAL(clicked()), this,SLOT(on_apply_clicked()));
}

WidSetLocal::~WidSetLocal()
{
    qDebug() << __FUNCTION__;
}

void WidSetLocal::on_apply_clicked()
{
    qDebug() << __FUNCTION__;
    qDebug() << "ip, port" << m_LE_ip->text() <<"," << m_LE_port->text();
}
