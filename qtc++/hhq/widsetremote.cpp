#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QtDebug>
#include <QMessageBox>
#include "widsetremote.h"
#include "hudpproc.h"
#include "mylog.h"
#include "hsettings.h"


WidSetRemote::WidSetRemote(int id, QWidget *parent) : QWidget(parent),m_id(id)
{
    qDebug() << __FUNCTION__;

    QLabel *lab_ip = new QLabel("IP", this);
    m_LE_ip = new QLineEdit("", this);
    QLabel *lab_port  = new QLabel("Port", this);
    m_LE_port = new QLineEdit("", this);

    //network address
    QGroupBox *groupBox_addr = new QGroupBox("Network Address", this);
    QFormLayout *flayout = new QFormLayout(groupBox_addr);
    flayout->setSpacing(15);
    flayout->addRow(lab_ip, m_LE_ip);
    flayout->addRow(lab_port, m_LE_port);

    qDebug() << __FUNCTION__ << 1;

    //configurations

    m_groupBox_cfg = new QGroupBox("Module Enablement", this);
    m_powerMod1 = new QCheckBox("Power Module 1");
    m_powerMod2 = new QCheckBox("Power Module 2");
    m_powerAmp1 = new QCheckBox("Power Amplifier 1");
    m_powerAmp2 = new QCheckBox("Power Amplifier 2");
    m_vol_slider = new QSlider();
    QFormLayout *flayout_vol = new QFormLayout();
    m_vol_slider->setOrientation(Qt::Horizontal);
    m_vol_slider->setMaximum(10);
    m_vol_slider->setMinimum(0);
    m_vol_slider->setTickInterval(1);
    m_vol_slider->setSingleStep(1);
    m_vol_slider->setPageStep(1);
    m_vol_slider->setTickPosition(QSlider::TicksBelow);
    connect(m_vol_slider, SIGNAL(valueChanged(int)), this, SLOT(on_volSiliderChanged(int)));
    flayout_vol->addRow("Volume", m_vol_slider);
    flayout_vol->setSpacing(30);

    //default value:
    QString cfg_tips = "This group is NOT checked by default, \n"
                       "which means these parameter are controlled\n"
                       "by system automatically; When it's checked \n"
                       "by user, it means users wants to control them\n"
                       "by themselves. i.e.\n"
                       "    unchecked:  system auto-mode\n"
                       "    checked:    user manual-mode\n";
    m_groupBox_cfg->setCheckable(true);
    m_groupBox_cfg->setChecked(false);
    m_groupBox_cfg->setToolTip(cfg_tips);
    m_powerMod1->setCheckState(Qt::Checked);
    m_powerMod2->setCheckState(Qt::Checked);
    m_powerAmp1->setCheckState(Qt::Checked);
    m_powerAmp2->setCheckState(Qt::Checked);
    m_vol_slider->setValue(7);
    qDebug() << __FUNCTION__ << 2;

    QFormLayout *flayout_cfg = new QFormLayout();
    flayout_cfg->addWidget(m_powerMod1);
    flayout_cfg->addWidget(m_powerMod2);
    flayout_cfg->addWidget(m_powerAmp1);
    flayout_cfg->addWidget(m_powerAmp2);
    flayout_cfg->addItem(flayout_vol);
    m_groupBox_cfg->setLayout(flayout_cfg);


    qDebug() << __FUNCTION__ << 3;

    //apply buttone
    m_bt_apply_local = new QPushButton("Apply", this);
    connect(m_bt_apply_local, SIGNAL(clicked()), this,SLOT(on_apply_clicked()));

    QGridLayout *gridLay = new QGridLayout(this);

    gridLay->addWidget(groupBox_addr, 0, 0);
    gridLay->addWidget(m_groupBox_cfg, 1, 0);
    gridLay->addWidget(m_bt_apply_local, 2, 0);


    //track the change of the configuration


    qDebug() << __FUNCTION__ << 4;

    //load config
    load_config();

    //
    //  status
    //
    // 1. network status
    m_LE_packetRx = new QLineEdit("", this);
    m_LE_packetTx = new QLineEdit("", this);
    m_LE_byteRx = new QLineEdit("", this);
    m_LE_byteTx = new QLineEdit("", this);
    QGroupBox *groupBox_netStatus = new QGroupBox("Network Status", this);

    QFormLayout *flayout_ns = new QFormLayout(groupBox_netStatus);
    flayout_ns->setSpacing(15);
    flayout_ns->addRow("Packets Received:", m_LE_packetRx);
    flayout_ns->addRow("Packets Sent:", m_LE_packetTx);
    flayout_ns->addRow("Bytes Received:", m_LE_byteRx);
    flayout_ns->addRow("Bytes Sent:", m_LE_byteTx);



    // 2. emitter status
     QGroupBox *groupBox_emitterState = new QGroupBox("Emitter && Electronic Bay");
    QFormLayout *flayout_status = new QFormLayout(groupBox_emitterState);
    m_sts_emitter_temp = new QLineEdit();
    m_sts_elec_insulation = new QLineEdit();
    flayout_status->addRow(tr("Emitter-Bay\nTemprature(℃)"), m_sts_emitter_temp);
    flayout_status->addRow(tr("Electronic-Bay\nInsulation Resistence(kΩ)"), m_sts_elec_insulation);
    flayout_status->setFormAlignment(Qt::AlignCenter);

    qDebug() << __FUNCTION__ << 5;

    // 3. module working status
    QGroupBox *groupBox_workStatus = new QGroupBox("Working Status");
    QFormLayout *flayout_workStatus = new QFormLayout(groupBox_workStatus);
    m_sts_powerMod1 = new QLineEdit();
    m_sts_powerMod2 = new QLineEdit();
    m_sts_powerAmp1 = new QLineEdit();
    m_sts_powerAmp2 = new QLineEdit();
    flayout_workStatus->addRow("Power Moduel 1", m_sts_powerMod1);
    flayout_workStatus->addRow("Power Module 2", m_sts_powerMod2);
    flayout_workStatus->addRow("Power Amplifier 1", m_sts_powerAmp1);
    flayout_workStatus->addRow("Power Amplifier 2", m_sts_powerAmp2);

    qDebug() << __FUNCTION__ << 6;
    // 4. output voltage
    QGroupBox *groupBox_outputAvgVolt = new QGroupBox("Average Output Voltage");
    QFormLayout *flayout_outputAvgVolt = new QFormLayout(groupBox_outputAvgVolt);
    m_outputAvgVolt_pa1 = new QLineEdit();
    m_outputAvgVolt_pa2 = new QLineEdit();
    m_outputAvgVolt_pa3 = new QLineEdit();
    m_outputAvgVolt_pa4 = new QLineEdit();
    flayout_outputAvgVolt->addRow("1st Power Amplifier", m_outputAvgVolt_pa1);
    flayout_outputAvgVolt->addRow("2nd Power Amplifier", m_outputAvgVolt_pa2);
    flayout_outputAvgVolt->addRow("3rd Power Amplifier", m_outputAvgVolt_pa3);
    flayout_outputAvgVolt->addRow("4th Power Amplifier", m_outputAvgVolt_pa4);

    gridLay->addWidget(groupBox_netStatus, 0, 1);
    gridLay->addWidget(groupBox_emitterState, 1, 1);
    gridLay->addWidget(groupBox_workStatus, 2, 1);
    gridLay->addWidget(groupBox_outputAvgVolt, 3,1);

    gridLay->setRowStretch(0, 1);
    gridLay->setRowStretch(1, 1);
    gridLay->setRowStretch(2, 2);
    gridLay->setRowStretch(3, 2);

    //not able to edit
    groupBox_netStatus->setDisabled(true);
    groupBox_emitterState->setDisabled(true);
    groupBox_workStatus->setDisabled(true);
    groupBox_outputAvgVolt->setDisabled(true);


    connect(this, SIGNAL(remoteChanged(int)),
            g_udpProc, SLOT(on_remoteChanged(int)));

    qDebug() << __FUNCTION__ << 7;

    //start a timer to update the  status
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL (timeout()), this, SLOT(on_statusTimerout()));

    if(is_validRemote())
    {
        m_timer->start(505);
    }
}

void WidSetRemote::load_config()
{
    m_LE_ip->setText(g_settings->remote_ip(m_id));
    m_LE_port->setText(g_settings->remote_port(m_id));
    if(g_settings->remote_enableMode_v(m_id) ==HSettings::e_enableMode_auto)
        m_groupBox_cfg->setChecked(false);
    else
        m_groupBox_cfg->setChecked(true);
    if(g_settings->remote_powerMod1_v(m_id) == HSettings::e_module_enabled)
        m_powerMod1->setCheckState(Qt::Checked);
    else
        m_powerMod1->setCheckState(Qt::Unchecked);

    if(g_settings->remote_powerMod2_v(m_id) == HSettings::e_module_enabled)
        m_powerMod2->setCheckState(Qt::Checked);
    else
        m_powerMod2->setCheckState(Qt::Unchecked);

    if(g_settings->remote_powerAmp1_v(m_id) == HSettings::e_module_enabled)
        m_powerAmp1->setCheckState(Qt::Checked);
    else
        m_powerAmp1->setCheckState(Qt::Unchecked);

    if(g_settings->remote_powerAmp2_v(m_id) == HSettings::e_module_enabled)
        m_powerAmp2->setCheckState(Qt::Checked);
    else
        m_powerAmp2->setCheckState(Qt::Unchecked);

    m_vol_slider->setValue(g_settings->remote_volume(m_id));
}

void WidSetRemote::on_apply_clicked()
{
    qDebug()<< "WidSetRemote::"<< __FUNCTION__<< "ip, port" << m_LE_ip->text() <<"," << m_LE_port->text();

    /*
     *   Note: ipStr or PortStr is empty means this remote is disabled.
    */

    QString ipStr = m_LE_ip->text().trimmed();
    QHostAddress addr(ipStr);
    if(addr.isNull() && ipStr.length() > 0)
    {
        QMessageBox::information(this,
                                 "Remote Network Address",
                                 "The input IPv4 Address is invalid !");
        m_LE_ip->setText(g_settings->remote_ip(m_id));

        return;
    }
    QString portStr = m_LE_port->text().trimmed();
    int port = portStr.toInt();
    if((port > 65000 || port < 1024) && portStr.length() > 0  && portStr != "0")
    {
        QMessageBox::information(this,
                                 "Remote Network Address",
                                 "The input port is invalid !\n"
                                 "The valid range is 1024 ~ 65000");
        m_LE_port->setText(g_settings->remote_port(m_id));
        return;
    }

    //save config
    g_settings->set_remote_ip(m_id, addr.toString());
    g_settings->set_remote_port(m_id, port);
    QString val;
    if(m_groupBox_cfg->isChecked())
        val = g_settings->val2Str_enableMode(HSettings::e_enableMode_manual);
    else
        val = g_settings->val2Str_enableMode(HSettings::e_enableMode_auto);
    g_settings->set_remote_enableMode(m_id, val);

    if(m_powerMod1->isChecked())
        val = g_settings->val2Str_enableValue(HSettings::e_module_enabled);
    else
        val = g_settings->val2Str_enableValue(HSettings::e_module_disabled);
    g_settings->set_remote_powerMod1(m_id, val);

    if(m_powerMod2->isChecked())
        val = g_settings->val2Str_enableValue(HSettings::e_module_enabled);
    else
        val = g_settings->val2Str_enableValue(HSettings::e_module_disabled);
    g_settings->set_remote_powerMod2(m_id, val);


    if(m_powerAmp1->isChecked())
        val = g_settings->val2Str_enableValue(HSettings::e_module_enabled);
    else
        val = g_settings->val2Str_enableValue(HSettings::e_module_disabled);
    g_settings->set_remote_powerAmp1(m_id, val);


    if(m_powerAmp2->isChecked())
        val = g_settings->val2Str_enableValue(HSettings::e_module_enabled);
    else
        val = g_settings->val2Str_enableValue(HSettings::e_module_disabled);
    g_settings->set_remote_powerAmp2(m_id, val);

    g_settings->set_remote_volume(m_id, m_vol_slider->value());

    g_settings->save();


    // update timer
    if(is_validRemote())
    {
        if(!m_timer->isActive())
        {
            m_timer->start(500);
        }
    }
    else
    {
        if(m_timer->isActive())
        {
            m_timer->stop();
            clear_status();
        }
    }


    emit remoteChanged(m_id);

}


bool WidSetRemote::is_validRemote()
{
    QHostAddress remote_ip(g_settings->remote_ip(m_id));
    quint16 remote_port= quint16(g_settings->remote_port(m_id).toInt());
    if(!remote_ip.isNull() && remote_port > 0)
    {
        return true;
    }
    return false;
}

void WidSetRemote::clear_status()
{
    m_LE_packetRx->clear();
    m_LE_packetTx->clear();
    m_LE_byteRx->clear();
    m_LE_byteTx->clear();

    //other status
    m_sts_emitter_temp->clear();
    m_sts_elec_insulation->clear();
    m_sts_powerMod1->clear();
    m_sts_powerMod2->clear();
    m_sts_powerAmp1->clear();
    m_sts_powerAmp2->clear();

    m_outputAvgVolt_pa1->clear();
    m_outputAvgVolt_pa2->clear();
    m_outputAvgVolt_pa3->clear();
    m_outputAvgVolt_pa4->clear();
}
void WidSetRemote::on_statusTimerout()
{
    //qDebug() << "WidSetRemote::" <<__FUNCTION__ << "id = " << m_id;
    //network status
    m_LE_packetRx->setText(QString::number(g_udpProc->RMpacketRx(m_id)));
    m_LE_packetTx->setText(QString::number(g_udpProc->RMpacketTx(m_id)));
    m_LE_byteRx->setText(QString::number(g_udpProc->RMbytesRx(m_id)));
    m_LE_byteTx->setText(QString::number(g_udpProc->RMbytesTx(m_id)));

    //other status
    m_sts_emitter_temp->setText(g_udpProc->emitter_temp_str(m_id));
    m_sts_elec_insulation->setText(QString::number(g_udpProc->insulation_resistance(m_id)));
    m_sts_powerMod1->setText(g_udpProc->powerMod1(m_id));
    m_sts_powerMod2->setText(g_udpProc->powerMod2(m_id));
    m_sts_powerAmp1->setText(g_udpProc->powerAmp1(m_id));
    m_sts_powerAmp2->setText(g_udpProc->powerAmp2(m_id));

    m_outputAvgVolt_pa1->setText(QString::number(g_udpProc->avgOutVoltag_amp1(m_id)));
    m_outputAvgVolt_pa2->setText(QString::number(g_udpProc->avgOutVoltag_amp2(m_id)));
    m_outputAvgVolt_pa3->setText(QString::number(g_udpProc->avgOutVoltag_amp3(m_id)));
    m_outputAvgVolt_pa4->setText(QString::number(g_udpProc->avgOutVoltag_amp4(m_id)));

}

void WidSetRemote::on_volSiliderChanged(int val)
{
    qDebug() << __FUNCTION__ << val;
    m_vol_slider->setToolTip(QString::number(val));

}
WidSetRemote::~WidSetRemote()
{
    qDebug() << __FUNCTION__;
}
