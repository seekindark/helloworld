#ifndef WIDDASHBOARD_H
#define WIDDASHBOARD_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include "btnplay.h"


class WidDashboard : public QWidget
{
    Q_OBJECT

public:
    explicit WidDashboard(QWidget *parent = nullptr);
    ~WidDashboard();


signals:

public slots:
   void on_played(BtnPlay *btn);
   void on_stopped(BtnPlay *btn);

private:
    QLabel *m_label_warning;
    QHBoxLayout *m_layout_warns;

    BtnPlay *m_bt_eng;
    BtnPlay *m_bt_local;
    BtnPlay *m_bt_siren;

    QLabel *m_label_tones;
    QHBoxLayout *m_layout_tones;
    BtnPlay *m_bt_tone1;
    BtnPlay *m_bt_tone2;
    BtnPlay *m_bt_tone3;
    BtnPlay *m_bt_tone4;
    BtnPlay *m_bt_tone5;

    QVBoxLayout *m_layout_all;

};

#endif // WIDDASHBOARD_H
