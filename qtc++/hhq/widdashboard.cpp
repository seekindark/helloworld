#include <QDebug>
#include <QProgressBar>
#include "widdashboard.h"

WidDashboard::WidDashboard(QWidget *parent) : QWidget(parent)
{
    QFont font;
    font.setFamily(QString::fromUtf8("Arial"));
    font.setPointSize(13);
    font.setBold(false);

    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    //QSize size = QSize(120, 90);
    //
    //Warnings
    //
    m_label_warning = new QLabel("WARNINGS", this);
    m_label_warning->setMaximumHeight(30);
    m_label_warning->setStyleSheet(QString::fromUtf8("background-color:rgb(84, 207, 252) ;\n"
                                                 "color: rgb(60, 60, 160);\n"));
    m_label_warning->setAlignment(Qt::AlignLeft |Qt:: AlignVCenter);
    m_label_warning->setFont(font);

    m_layout_warns = new QHBoxLayout();
    //m_layout_warns->setGeometry(QRect(0, 20+30, 700, 200));
    //m_layout_warns->setContentsMargins(20,20,50,20);

    QLabel *dummy_lable1 = new QLabel("" );
    dummy_lable1->setSizePolicy(sizePolicy);

    m_bt_eng = new BtnPlay("ENGLISH",  BT_WARNING_ENG, this);
    m_bt_local = new BtnPlay("LOCAL",  BT_WARNING_LOCAL, this);
    m_bt_siren = new BtnPlay("SIREN",  BT_WARNING_SIREN, this);
    m_bt_warnNA = new BtnPlay("OTHER...",  BT_WARNING_NA, this);



    m_layout_warns->addWidget(m_bt_eng);
    m_layout_warns->addWidget(m_bt_local);
    m_layout_warns->addWidget(m_bt_siren);
    m_layout_warns->addWidget(m_bt_warnNA);
    m_layout_warns->addWidget(dummy_lable1);
    m_layout_warns->addWidget(dummy_lable1);
    m_layout_warns->setSpacing(15);

    //
    //tones
    //
    m_label_tones = new QLabel("DISRUPTING FREQUENCY-TONES", this);
    //m_label_tones->setGeometry(QRect(0, 220, 700, 30));
    m_label_tones->setMaximumHeight(30);
    m_label_tones->setFont(font);
    m_label_tones->setStyleSheet(QString::fromUtf8("background-color:rgb(84, 207, 252) ;\n"
                                                 "color: rgb(60, 60, 160);\n"));
    m_label_tones->setAlignment(Qt::AlignLeft |Qt:: AlignVCenter);
    m_label_tones->setFont(font);

    m_layout_tones = new QGridLayout();
    m_layout_tones->setSpacing(15);
    m_bt_tone1 = new BtnPlay("TONE-1",  BT_TONE_1, this);
    m_bt_tone2 = new BtnPlay("TONE-2",  BT_TONE_2, this);
    m_bt_tone3 = new BtnPlay("TONE-3",  BT_TONE_3, this);
    m_bt_tone4 = new BtnPlay("TONE-4",  BT_TONE_4, this);
    m_bt_tone5 = new BtnPlay("TONE-5",  BT_TONE_5, this);
    m_bt_tone6 = new BtnPlay("TONE-6",  BT_TONE_6, this);
    m_bt_toneNA = new BtnPlay("OTHER...",  BT_TONE_NA, this);
    QLabel *dummy_tone = new QLabel();

    m_layout_tones->addWidget(m_bt_tone1, 0, 0);
    m_layout_tones->addWidget(m_bt_tone2, 0, 1);
    m_layout_tones->addWidget(m_bt_tone3, 0, 2);
    m_layout_tones->addWidget(m_bt_toneNA, 0, 3);
    m_layout_tones->addWidget(dummy_tone, 0, 5);
    m_layout_tones->addWidget(m_bt_tone4, 1, 0);
    m_layout_tones->addWidget(m_bt_tone5, 1, 1);
    m_layout_tones->addWidget(m_bt_tone6, 1, 2);


    m_layout_all = new QVBoxLayout(this);
    //m_layout_all->setContentsMargins(120,20,0,100);
    m_layout_all->setSpacing(20);
    m_layout_all->addWidget(m_label_warning, 50);
    m_layout_all->addLayout(m_layout_warns, 200);
    m_layout_all->addWidget(m_label_tones, 50);
    m_layout_all->addLayout(m_layout_tones,350);
    QLabel *dummy_lable3 = new QLabel("" ,this);
    m_layout_all->addWidget(dummy_lable3, 200);

     m_layout_all->setContentsMargins(0,20,0,0);
     setLayout(m_layout_all);

}

void WidDashboard::on_played(BtnPlay *btn)
{
    qDebug() << "widDashbod:" << __func__ <<": "<< btn->text();
    btn->set_progress(50);

}
void WidDashboard::on_stopped(BtnPlay *btn)
{
    qDebug() << "widDashbod:" << __func__ <<": "<< btn->text();
    btn->set_progress(0);
}

WidDashboard::~WidDashboard()
{
    qDebug() <<__FUNCTION__;
}

void WidDashboard::load_fileList(FileTableWidgetItemList_R &fileList)
{
    qDebug(__FUNCTION__);

    //clear NA filelist firstly
    m_bt_warnNA->clear_soundFileNA();
    m_bt_toneNA->clear_soundFileNA();
    // clear it firstly
    m_bt_eng->set_soundFile(nullptr);
    m_bt_local->set_soundFile(nullptr);
    m_bt_siren->set_soundFile(nullptr);

    m_bt_tone1->set_soundFile(nullptr);
    m_bt_tone2->set_soundFile(nullptr);
    m_bt_tone3->set_soundFile(nullptr);
    m_bt_tone4->set_soundFile(nullptr);
    m_bt_tone5->set_soundFile(nullptr);
    m_bt_tone6->set_soundFile(nullptr);
    //update new filelist
    FileTableWidgetItemList_R_itor it;
    for (it = fileList.begin(); it != fileList.end(); it++)
    {
        const FileTableWidgetItem * item = *it;
        //qDebug() << __FUNCTION__ <<item->text();
        if(item->is_warnig())
        {
            switch(item->aliasIdx())
            {
                case FileTableWidgetItem::e_warning_english:
                    m_bt_eng->set_soundFile(item);
                    break;
                case FileTableWidgetItem::e_warning_siren:
                    m_bt_siren->set_soundFile(item);
                    break;
                case FileTableWidgetItem::e_warning_local:
                    m_bt_local->set_soundFile(item);
                    break;
                case FileTableWidgetItem::e_warning_NA:
                    m_bt_warnNA->add_soundFileNA(item);
                    break;
                default:
                 break;
            }
        }
        else if(item->is_tone())
        {
            switch(item->aliasIdx())
            {
                case FileTableWidgetItem::e_tone_1:
                    m_bt_tone1->set_soundFile(item);
                break;
                case FileTableWidgetItem::e_tone_2:
                    m_bt_tone2->set_soundFile(item);
                break;
                case FileTableWidgetItem::e_tone_3:
                    m_bt_tone3->set_soundFile(item);
                break;
                case FileTableWidgetItem::e_tone_4:
                    m_bt_tone4->set_soundFile(item);
                break;
                case FileTableWidgetItem::e_tone_5:
                    m_bt_tone5->set_soundFile(item);
                break;
                case FileTableWidgetItem::e_tone_6:
                    m_bt_tone6->set_soundFile(item);
                break;
                case FileTableWidgetItem::e_tone_NA:
                    m_bt_toneNA->add_soundFileNA(item);
                break;
                default:
                break;
            }
        }
    }

    //set NA soundfiles tips
    m_bt_warnNA->setTips_soundFileNA();
    m_bt_toneNA->setTips_soundFileNA();
}
