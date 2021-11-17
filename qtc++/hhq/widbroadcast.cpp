#include <QDebug>
#include "widbroadcast.h"

WidBroadCast::WidBroadCast(QWidget *parent) : QWidget(parent)
{
    qDebug() << __FUNCTION__;

    QFont font;
    font.setFamily(QString::fromUtf8("Arial"));
    font.setPointSize(13);
    font.setBold(false);

    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    QSize size = QSize(120, 90);
    //
    //broadcast
    //
    m_lab_broadcast = new QLabel("BROADCAST WITH MICROPHONE", this);
    m_lab_broadcast->setMaximumHeight(30);
    m_lab_broadcast->setStyleSheet(QString::fromUtf8("background-color:rgb(84, 207, 252) ;\n"
                                                 "color: rgb(60, 60, 160);\n"));
    m_lab_broadcast->setAlignment(Qt::AlignLeft |Qt:: AlignVCenter);
    m_lab_broadcast->setFont(font);
    m_lab_broadcast->setSizePolicy(sizePolicy);

    //button mic
    m_bt_mic = new QPushButton(this);
    m_bt_mic->setStyleSheet(QString::fromUtf8("background-color:rgb(84, 207, 252) ;\n"
                                                 "color: rgb(60, 60, 160);\n"));
    m_bt_mic->setFont(font);
    m_bt_mic->setSizePolicy(sizePolicy);
    m_bt_mic->setMaximumSize(size);
    QHBoxLayout *hlay_mic = new QHBoxLayout();
    //hlay_mic->setAlignment(Qt::AlignHCenter);
    hlay_mic->addWidget(m_bt_mic);
    hlay_mic->setContentsMargins(0,50,0,50);
    m_bt_mic->setIcon(QIcon(":/main/resource/mic-off.ico"));
    m_bt_mic->setIconSize(QSize(64,64));
    connect(m_bt_mic, SIGNAL(clicked()), this, SLOT(on_mic_clicked()));

    //prompt
    m_lab_prompt = new QLabel("\n  You're talking to the underwater speakers real-timely !\n", this);
    m_lab_prompt->setStyleSheet(QString::fromUtf8("background-color:rgb(255, 170, 127) ;\n"
                                                 "color: rgb(255, 0, 0);\n"));
    m_lab_prompt->setAlignment(Qt::AlignLeft |Qt:: AlignTop);
    font.setPointSize(15);
    font.setBold(true);
    m_lab_prompt->setFont(font);
    m_lab_prompt->setMaximumHeight(70);
    m_lab_prompt->setSizePolicy(sizePolicy);
    //tips (how to)
    m_lable_tips = new QLabel(m_lab_prompt);
    m_lable_tips->setText("   Press the record button and speak into the microphone.\n"
                          "   Press the butteon again to stop broadcasting.");
    m_lable_tips->setStyleSheet(QString::fromUtf8("background-color:rgb(255, 170, 127) ;\n"
                                                 "color: rgb(255, 0, 0);\n"));
    m_lable_tips->setAlignment(Qt::AlignLeft |Qt:: AlignTop);
    font.setPointSize(13);
    font.setBold(false);
    m_lable_tips->setFont(font);
    m_lable_tips->setMaximumHeight(70);
    m_lable_tips->setSizePolicy(sizePolicy);

    QVBoxLayout *vlayout_tips = new QVBoxLayout();
    vlayout_tips->setSpacing(0);
    vlayout_tips->addWidget(m_lab_prompt);
    vlayout_tips->addWidget(m_lable_tips);



    m_layout_all = new QVBoxLayout(this);
    //m_layout_all->setContentsMargins(120,20,0,100);
    m_layout_all->setSpacing(20);
    m_layout_all->addWidget(m_lab_broadcast, 1);
    m_layout_all->addLayout(hlay_mic, 4);
    m_layout_all->addLayout(vlayout_tips, 4);
    m_layout_all->setContentsMargins(0,20,0,0);
    m_layout_all->setAlignment(Qt::AlignTop);
    setLayout(m_layout_all);

    m_is_on = false;
    m_pb_h = nullptr;
    m_pb_v = nullptr;
}

void WidBroadCast::on_mic_clicked()
{
    if(m_is_on)
    {
        qDebug() <<__FUNCTION__ <<"to off the mic";
        m_is_on = false;
        //to off the microphone
        //to start-on the microphone
        m_bt_mic->setIcon(QIcon(":/main/resource/mic-off.ico"));
        m_bt_mic->setIconSize(QSize(64,64));

        if(m_pb_h != nullptr)
        {
            m_pb_h->hide();
        }
        if(m_pb_v != nullptr)
        {
            m_pb_v->hide();
        }
    }
    else
    {
        qDebug() <<__FUNCTION__ <<"to on the mic";
        m_is_on = true;
        //to start-on the microphone
        m_bt_mic->setIcon(QIcon(":/main/resource/mic-on.ico"));
        m_bt_mic->setIconSize(QSize(64,64));

        //horizontal progress bar
        if(m_pb_h == nullptr)
        {
            m_pb_h = new QProgressBar(m_bt_mic);
            m_pb_h->setGeometry(0,m_bt_mic->height()-5,m_bt_mic->width()-2, 5);
            m_pb_h->setWindowFlags(Qt::FramelessWindowHint);
            m_pb_h->setAttribute(Qt::WA_TranslucentBackground);
            m_pb_h->setStyleSheet("background-color: rgb(84, 207, 252);\n"
                                  "border-radius:3px;\n");
            m_pb_h->setTextVisible(false);
            m_pb_h->setValue(50);
            m_pb_h->show();
        }
        else
        {
            m_pb_h->show();
        }
        //vertical progress bar
        if(m_pb_v == nullptr)
        {
            m_pb_v = new QProgressBar(m_bt_mic);
            m_pb_v->setOrientation(Qt::Vertical);
            m_pb_v->setWindowFlags(Qt::FramelessWindowHint);
            m_pb_v->setAttribute(Qt::WA_TranslucentBackground);
            m_pb_v->setStyleSheet("background-color: rgb(84, 207, 252);\n"
                                  "border-radius:3px;\n");
            m_pb_v->setGeometry(m_bt_mic->width()/2 - 5, 18 , 10, m_bt_mic->height()*1/3+2);
            m_pb_v->setTextVisible(false);
            m_pb_v->setValue(60);
            m_pb_v->show();

        }
        else
        {
            m_pb_v->show();
        }
    }
}
