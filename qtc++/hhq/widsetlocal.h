#ifndef WIDSETLOCAL_H
#define WIDSETLOCAL_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>


class WidSetLocal : public QWidget
{
    Q_OBJECT
public:
    explicit WidSetLocal(QWidget *parent = nullptr);
    ~WidSetLocal();
    void load_config();

signals:
    void localAddr_changed();

public slots:
    void on_apply_clicked();
    void on_statusTimerout();

private:




    QLineEdit *m_LE_ip;
    QLineEdit *m_LE_port;
    QPushButton *m_bt_apply_local;

    //status side
    QLineEdit *m_LE_sockState;
    QLineEdit *m_LE_packetRx;
    QLineEdit *m_LE_packetTx;
    QLineEdit *m_LE_byteRx;
    QLineEdit *m_LE_byteTx;

    QTimer *m_timer;
};

#endif // WIDSETLOCAL_H
