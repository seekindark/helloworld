#ifndef WIDDASHBOARD_H
#define WIDDASHBOARD_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include "btnplay.h"
#include "filetablewidgetitem.h"

class WidDashboard : public QWidget
{
    Q_OBJECT

public:
    explicit WidDashboard(QWidget *parent = nullptr);
    ~WidDashboard();

    void load_fileList(FileTableWidgetItemList_R &fileList);

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
    BtnPlay *m_bt_warnNA;

    QLabel *m_label_tones;
    QGridLayout *m_layout_tones;
    BtnPlay *m_bt_tone1;
    BtnPlay *m_bt_tone2;
    BtnPlay *m_bt_tone3;
    BtnPlay *m_bt_tone4;
    BtnPlay *m_bt_tone5;
    BtnPlay *m_bt_tone6;
    BtnPlay *m_bt_toneNA;

    QVBoxLayout *m_layout_all;

};

#endif // WIDDASHBOARD_H
