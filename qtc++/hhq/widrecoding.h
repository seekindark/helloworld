#ifndef WIDRECODING_H
#define WIDRECODING_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QProgressBar>

class WidRecoding : public QWidget
{
    Q_OBJECT
public:
    explicit WidRecoding(QWidget *parent = nullptr);
    void reset();


    typedef enum
    {
        e_mic_off = 0,
        e_mic_recording,   // when recording is done, it goes to 'review-ready' directly, and show review page!!
        e_mic_review_ready,
        e_mic_reviewing   // when reviewing is done, it goes back to 'ready' until 'discard' button is clicked.
    }MIC_state;

    typedef enum
    {
        e_prepare_new,
        e_prepare_back
    }E_PREPARE_type;
signals:
    void recordingSaved(const QStringList &file);

public slots:
    void on_mic_clicked();
    void on_bt_discard_clicked();
    void on_bt_save_clicked();


private:

    QString m_sound_dir_default;

    QLabel *m_lab_recording;
    QVBoxLayout *m_layout_all;

    //1st part
    QPushButton *m_bt_mic;
    QProgressBar *m_pb_v;  // used for recording progress
    QProgressBar *m_pb_h;   // used for review(play out) progress

    //2nd part (a layout)
    QVBoxLayout *m_layout_2ndPart;

    //case 1: recording USAGE
    QLabel *m_lab_prompt;
    QLabel *m_lable_tips;

    //case 2: warning soude review
    QLabel *m_lab_fileName;
    QLineEdit *m_le_fileName;
    QCheckBox *m_chk_repeat;
    QComboBox *m_comBox_type;
    QPushButton *m_bt_discard;
    QPushButton *m_bt_save;
    QHBoxLayout *m_lay_review_1stline;
    QHBoxLayout *m_lay_revaiew_2ndline;

    //state
    MIC_state m_state;

    void prepare_recording();
    void start_recording();
    void prepare_review(E_PREPARE_type type = e_prepare_new);
    void start_reviewing();


    void show_2ndPart_record(bool show_flag=true);
    void show_2ndPart_review(bool show_flag=true);
    void hide_2ndPart_record(){show_2ndPart_record(false);}
    void hide_2ndPart_review(){show_2ndPart_review(false);}

    bool check_and_save_recording();
    void check_default_sound_dir();
    const QString warningAlias(int index);
};

#endif // WIDRECODING_H
