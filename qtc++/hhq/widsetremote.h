#ifndef WIDSETREMOTE_H
#define WIDSETREMOTE_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QSlider>
class WidSetRemote : public QWidget
{
    Q_OBJECT
public:
    explicit WidSetRemote(int id, QWidget *parent = nullptr);
    ~WidSetRemote();
    void load_config();

signals:
    void remoteChanged(int id);

public slots:
    void on_apply_clicked();
    void on_volSiliderChanged(int val);
    void on_statusTimerout();

private:

    bool is_validRemote();
    void clear_status();


    int m_id;   // 1: remote-1, 2: remote-2

    //address
    QLineEdit *m_LE_ip;
    QLineEdit *m_LE_port;

    // configuration
    QGroupBox *m_groupBox_cfg;
    QCheckBox *m_powerMod1;
    QCheckBox *m_powerMod2;
    QCheckBox *m_powerAmp1;
    QCheckBox *m_powerAmp2;
    QSlider *m_vol_slider;

    // status
    QLineEdit *m_sts_emitter_temp;
    QLineEdit *m_sts_elec_insulation;
    QLineEdit *m_sts_powerMod1;
    QLineEdit *m_sts_powerMod2;
    QLineEdit *m_sts_powerAmp1;
    QLineEdit *m_sts_powerAmp2;
    QLineEdit *m_outputAvgVolt_pa1;
    QLineEdit *m_outputAvgVolt_pa2;
    QLineEdit *m_outputAvgVolt_pa3;
    QLineEdit *m_outputAvgVolt_pa4;

    QPushButton *m_bt_apply_local;

    //network status
    QLineEdit *m_LE_packetRx;
    QLineEdit *m_LE_packetTx;
    QLineEdit *m_LE_byteRx;
    QLineEdit *m_LE_byteTx;


    QTimer *m_timer;
};

#endif // WIDSETREMOTE_H
