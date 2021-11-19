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

signals:

public slots:
    void on_apply_clicked();


private:
    QLineEdit *m_LE_ip;
    QLineEdit *m_LE_port;
    QPushButton *m_bt_apply_local;

};

#endif // WIDSETLOCAL_H
