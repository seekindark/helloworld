#ifndef BTNPLAY_H
#define BTNPLAY_H

#include <QPushButton>
#include <QObject>
#include <QProgressBar>

typedef enum{
    BT_WARNING_ENG = 1,
    BT_WARNING_LOCAL,
    BT_WARNING_SIREN,
    BT_TONE_1,
    BT_TONE_2,
    BT_TONE_3,
    BT_TONE_4,
    BT_TONE_5,
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
    bool is_tone(){return (m_type >= BT_TONE_1 && m_type <= BT_TONE_5);}
signals:
    void played(BtnPlay *obj);
    void stopped(BtnPlay *obj);

public slots:
    void on_clicked();

private:
    BT_type m_type;
    QProgressBar *m_pb;
    bool m_is_played;
};

#endif // BTNPLAY_H
