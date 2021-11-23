#ifndef BTNPLAY_H
#define BTNPLAY_H

#include <QPushButton>
#include <QObject>
#include <QProgressBar>
#include <QComboBox>
#include <QLabel>
#include "filetablewidgetitem.h"

typedef enum{
    BT_WARNING_ENG = 1,
    BT_WARNING_LOCAL,
    BT_WARNING_SIREN,
    BT_WARNING_NA,
    BT_TONE_1,
    BT_TONE_2,
    BT_TONE_3,
    BT_TONE_4,
    BT_TONE_5,
    BT_TONE_6,
    BT_TONE_NA,
    BT_INVALID
} BT_type;

class BtnPlay : public QPushButton
{
    Q_OBJECT
public:
    explicit BtnPlay(const QString &text, BT_type type,  QWidget *parent = nullptr);
    ~BtnPlay();


    void set_progress(int val){
        if(m_pb != nullptr) m_pb->setValue(val);}

    bool is_warning(){return (m_type == BT_WARNING_ENG ||
                              m_type == BT_WARNING_LOCAL||
                              m_type == BT_WARNING_SIREN);}
    bool is_tone(){return (m_type >= BT_TONE_1 && m_type <= BT_TONE_6);}
    bool is_warningNA(){return (m_type == BT_WARNING_NA);}
    bool is_toneNA(){return (m_type == BT_TONE_NA);}

    void set_soundFile(const FileTableWidgetItem *fItem);
    void add_soundFileNA(const FileTableWidgetItem *fItem);
    void clear_soundFileNA();
    void setTips_soundFileNA();

    const FileTableWidgetItem *fileWidItem_r();

signals:
    void played(BtnPlay *obj);
    void stopped(BtnPlay *obj);

public slots:
    void on_clicked();

    void on_NAFileBox_IndexChanged(int index);

private:
    QString m_tips_noFileSet;
    QString m_style_normal;
    QString m_style_SoundNotSet;


    BT_type m_type;
    QProgressBar *m_pb;
    bool m_is_played;

    // readonly, the file to be played
    const FileTableWidgetItem * m_soundFile;

    // filelist to be played in NA mode
    QComboBox * m_NAfileBox;
    QLabel *m_text_lable;
    FileTableWidgetItemList_R m_NAfileList;
};

#endif // BTNPLAY_H
