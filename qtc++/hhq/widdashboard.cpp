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



    m_layout_warns->addWidget(m_bt_eng);
    m_layout_warns->addWidget(m_bt_local);
    m_layout_warns->addWidget(m_bt_siren);
    m_layout_warns->addWidget(dummy_lable1);
    m_layout_warns->addWidget(dummy_lable1);
    m_layout_warns->addWidget(dummy_lable1);

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

    m_layout_tones = new QHBoxLayout();

    m_bt_tone1 = new BtnPlay("TONE-1",  BT_TONE_1, this);
    m_bt_tone2 = new BtnPlay("TONE-2",  BT_TONE_2, this);
    m_bt_tone3 = new BtnPlay("TONE-3",  BT_TONE_3, this);
    m_bt_tone4 = new BtnPlay("TONE-4",  BT_TONE_4, this);
    m_bt_tone5 = new BtnPlay("TONE-5",  BT_TONE_5, this);

    QLabel *dummy_lable2 = new QLabel("" ,this);
    m_layout_tones->addWidget(m_bt_tone1);
    m_layout_tones->addWidget(m_bt_tone2);
    m_layout_tones->addWidget(m_bt_tone3);
    m_layout_tones->addWidget(m_bt_tone4);
    m_layout_tones->addWidget(m_bt_tone5);
    m_layout_tones->addWidget(dummy_lable2);

    m_layout_all = new QVBoxLayout(this);
    //m_layout_all->setContentsMargins(120,20,0,100);
    m_layout_all->setSpacing(20);
    m_layout_all->addWidget(m_label_warning, 50);
    m_layout_all->addLayout(m_layout_warns, 200);
    m_layout_all->addWidget(m_label_tones, 50);
    m_layout_all->addLayout(m_layout_tones,200);
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

}
