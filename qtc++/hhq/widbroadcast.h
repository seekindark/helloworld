#ifndef WIDBROADCAST_H
#define WIDBROADCAST_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QProgressBar>

class WidBroadCast : public QWidget
{
    Q_OBJECT
public:
    explicit WidBroadCast(QWidget *parent = nullptr);


signals:

public slots:
    void on_mic_clicked();

private:
    QLabel *m_lab_broadcast;
    QVBoxLayout *m_layout_all;

    QPushButton *m_bt_mic;

    QLabel *m_lab_prompt;
    QLabel *m_lable_tips;

    bool m_is_on;

    QProgressBar *m_pb_h;
    QProgressBar *m_pb_v;

};

#endif // WIDBROADCAST_H
