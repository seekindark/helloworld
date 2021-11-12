#include <QtDebug>
#include "widrecoding.h"

WidRecoding::WidRecoding(QWidget *parent) : QWidget(parent)
{
    QFont font;
    font.setFamily(QString::fromUtf8("Arial"));
    font.setPointSize(13);
    font.setBold(false);

    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    QSize size = QSize(120, 90);
    //
    //1st part: recording (title) and MIC button
    //
    m_lab_recording = new QLabel("RECORD SOUND", this);
    m_lab_recording->setMaximumHeight(30);
    m_lab_recording->setStyleSheet(QString::fromUtf8("background-color:rgb(84, 207, 252) ;\n"
                                                 "color: rgb(60, 60, 160);\n"));
    m_lab_recording->setAlignment(Qt::AlignLeft |Qt:: AlignVCenter);
    m_lab_recording->setFont(font);
    m_lab_recording->setSizePolicy(sizePolicy);

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

    connect(m_bt_mic, SIGNAL(clicked()), this, SLOT(on_mic_clicked()));

    //
    // 2nd part: create prompt+tips for Recording usage
    //
    m_lab_prompt = new QLabel("\n  Your recording will be used later !\n");
    m_lab_prompt->setStyleSheet(QString::fromUtf8("background-color:rgb(84, 207, 252) ;\n"
                                                 "color: rgb(60, 60, 160);\n"));
    m_lab_prompt->setAlignment(Qt::AlignLeft |Qt:: AlignTop);
    font.setPointSize(15);
    font.setBold(true);
    m_lab_prompt->setFont(font);
    m_lab_prompt->setMaximumHeight(70);
    m_lab_prompt->setSizePolicy(sizePolicy);
    //tips (how to)
    m_lable_tips = new QLabel(m_lab_prompt);
    m_lable_tips->setText("   Press the record button and speak into the microphone.\n"
                          "   Press the butteon again to finish recording.");
    m_lable_tips->setStyleSheet(QString::fromUtf8("background-color:rgb(84, 207, 252) ;\n"
                                                 "color: rgb(60, 60, 160);\n"));
    m_lable_tips->setAlignment(Qt::AlignLeft |Qt:: AlignTop);
    font.setPointSize(13);
    font.setBold(false);
    m_lable_tips->setFont(font);
    m_lable_tips->setMaximumHeight(70);
    m_lable_tips->setSizePolicy(sizePolicy);

    m_layout_2ndPart = new QVBoxLayout();
    m_layout_2ndPart->setSpacing(0);

    //
    //whole part: to combin 1st+2nd part
    //
    m_layout_all = new QVBoxLayout(this);
    m_layout_all->setSpacing(20);
    m_layout_all->setContentsMargins(0,20,0,0);
    m_layout_all->setAlignment(Qt::AlignTop);
    //add 1st part
    m_layout_all->addWidget(m_lab_recording, 1);
    m_layout_all->addLayout(hlay_mic, 4);
    //set 2nd part stretch
    m_layout_all->setStretch(2, 4); //     m_layout_all->addLayout(m_layout_2ndPart, 4);
    setLayout(m_layout_all);

    //ready to recording
    prepare_recording();

    //members
    m_pb_v = nullptr;
    m_pb_h = nullptr;
}


void WidRecoding::on_mic_clicked()
{
    switch (m_state)
    {
        case e_mic_off:
            // switch to recording
            start_recording();
            break;
        case e_mic_recording:
            //switch to review-ready
            prepare_review();
        break;
        case e_mic_review_ready:
            //switch to reviewing
            start_reviewing();
        break;
        case e_mic_reviewing:
            //switch back to review-ready
            prepare_review(e_prepare_back);
        break;
        default:
        break;
    }

}
void WidRecoding::prepare_recording()
{
    m_state = e_mic_off;
    m_bt_mic->setIcon(QIcon(":/main/resource/mic-off.ico"));
    m_bt_mic->setIconSize(QSize(64,64));
    if(m_pb_v != nullptr)
    {
        m_pb_v->hide();
    }
    if(m_pb_h != nullptr)
    {
        m_pb_h->hide();
    }

    //switch to review-sound page
    m_lab_recording->setText("RECORD SOUND");

    hide_2ndPart_review();
    show_2ndPart_record();
}
void WidRecoding::start_recording()
{
    m_state = e_mic_recording;
    m_bt_mic->setIcon(QIcon(":/main/resource/mic-on.ico"));
    m_bt_mic->setIconSize(QSize(64,64));

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

void WidRecoding::prepare_review(E_PREPARE_type type)
{
    m_state = e_mic_review_ready;
    m_bt_mic->setIcon(QIcon(":/main/resource/reviewplay.ico"));
    m_bt_mic->setIconSize(QSize(128,128));
    if(m_pb_v != nullptr)
    {
        m_pb_v->hide();
    }
    if(m_pb_h != nullptr)
    {
        m_pb_h->hide();
    }

    //switch to review-sound page
    if(type == e_prepare_new)
    {
        m_lab_recording->setText("REVIEW SOUND");

        hide_2ndPart_record();
        show_2ndPart_review();
    }

}
void WidRecoding::start_reviewing()
{
    m_state = e_mic_reviewing;
    m_bt_mic->setIcon(QIcon(":/main/resource/reviewstop.ico"));
    m_bt_mic->setIconSize(QSize(128,128));

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
}

void WidRecoding::show_2ndPart_record(bool show_flag)
{
    if(show_flag)  // to show it
    {
        m_layout_2ndPart->addWidget(m_lab_prompt);
        m_layout_2ndPart->addWidget(m_lable_tips);
        m_layout_all->addLayout(m_layout_2ndPart);
        m_lab_prompt->show();
        m_lable_tips->show();
    }
    else // to hide it
    {
        //hide the label-prompt and tips which are used for recording page only
        m_layout_all->removeItem(m_layout_2ndPart);
        m_layout_2ndPart->removeWidget(m_lab_prompt);
        m_layout_2ndPart->removeWidget(m_lable_tips);
        m_lab_prompt->hide();
        m_lable_tips->hide();
    }
}
void WidRecoding::show_2ndPart_review(bool show_flag)
{
    if(show_flag)  // to show it
    {
        // if it's not created, then create them
        // label-filename can represent the 2nd part of review page
        if(m_lab_fileName == nullptr)
        {
            //create new 2nd-part of the page for review-sound
            QFont font;
            font.setFamily(QString::fromUtf8("Arial"));
            font.setPointSize(13);
            font.setBold(false);
            QString style = QString::fromUtf8("background-color:rgb(84, 207, 252) ;\n"
                            "color: rgb(60, 60, 160)\n");
            QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

            m_lab_fileName = new QLabel("Name");
            m_lab_fileName->setSizePolicy(sizePolicy);
            m_lab_fileName->setMaximumHeight(40);
            m_lab_fileName->setFont(font);
            m_lab_fileName->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

            m_le_fileName = new QLineEdit();
            m_le_fileName->setSizePolicy(sizePolicy);
            m_le_fileName->setMaximumHeight(40);
            m_le_fileName->setFont(font);
            m_le_fileName->setStyleSheet("background-color: white");

            m_chk_repeat = new QCheckBox("Repeat");
            m_chk_repeat->setSizePolicy(sizePolicy);
            m_chk_repeat->setMaximumHeight(40);
            //m_chk_repeat->setStyleSheet(style);
            //font.setPointSize(15);
            m_chk_repeat->setFont(font);
            //font.setPointSize(13);      //back to normal font size

            m_comBox_type = new QComboBox();
            m_comBox_type->setSizePolicy(sizePolicy);
            m_comBox_type->setMaximumHeight(40);
            m_comBox_type->setFont(font);
            m_comBox_type->setEditable(false);
            m_comBox_type->setStyleSheet(style);
            m_comBox_type->setPlaceholderText("Warning");
            m_comBox_type->addItems({"Warning English", "Warning Local", "Warning Siren", "N/A"});
            m_comBox_type->setCurrentIndex(0);
            qDebug() << "current comboBox =" << m_comBox_type->currentIndex();
            qDebug()<< "combo placeholder text = " << m_comBox_type->placeholderText();

            m_bt_discard = new QPushButton("DISCARD");
            m_bt_discard->setSizePolicy(sizePolicy);
            m_bt_discard->setMaximumHeight(40);
            m_bt_discard->setFont(font);
            m_bt_discard->setStyleSheet(style);
            connect(m_bt_discard, SIGNAL(clicked()), this, SLOT(on_bt_discard_clicked()));

            m_bt_save = new QPushButton("SAVE");
            m_bt_save->setSizePolicy(sizePolicy);
            m_bt_save->setMaximumHeight(40);
            m_bt_save->setFont(font);
            m_bt_save->setStyleSheet(style);
            connect(m_bt_save, SIGNAL(clicked()), this, SLOT(on_bt_save_clicked()));

            m_lay_review_1stline = new QHBoxLayout();
            m_lay_revaiew_2ndline = new QHBoxLayout();

            m_lay_review_1stline->setSpacing(20);
            m_lay_review_1stline->setContentsMargins(0, 30, 0, 30);
            m_lay_review_1stline->addWidget(m_lab_fileName, 1);
            m_lay_review_1stline->addWidget(m_le_fileName, 3);
            m_lay_review_1stline->addWidget(m_comBox_type,2);
            m_lay_review_1stline->addWidget(m_chk_repeat, 2);

            m_lay_revaiew_2ndline->addWidget(m_bt_discard, 1);
            m_lay_revaiew_2ndline->addWidget(m_bt_save, 1);
            m_lay_revaiew_2ndline->setSpacing(20);
            m_lay_revaiew_2ndline->setContentsMargins(0, 10, 0, 30);
        }

        //show them
        m_layout_2ndPart->addLayout(m_lay_review_1stline);
        m_layout_2ndPart->addLayout(m_lay_revaiew_2ndline);
        m_layout_all->addLayout(m_layout_2ndPart);
        m_bt_discard->show();
        m_bt_save->show();
        m_lab_fileName->show();
        m_le_fileName->show();
        m_comBox_type->show();
        m_chk_repeat->show();

    }
    else // to hide it
    {
        if(m_lab_fileName != nullptr)  // Just a protect from duplicated calling
        {
            m_layout_all->removeItem(m_layout_2ndPart);
            m_layout_2ndPart->removeItem(m_lay_review_1stline);
            m_layout_2ndPart->removeItem(m_lay_revaiew_2ndline);

            m_bt_discard->hide();
            m_bt_save->hide();
            m_lab_fileName->hide();
            m_le_fileName->hide();
            m_comBox_type->hide();
            m_chk_repeat->hide();
        }
    }
}

void WidRecoding::on_bt_discard_clicked()
{
    qDebug() << "WidRecoding::" << __FUNCTION__;

    if(m_state == e_mic_review_ready)
    {

    }
    else if(m_state == e_mic_reviewing)
    {
        //stop the review, i.e. stop the music playing
    }

    //switch back to prepar-recording, i.e. mic-off STATE
    prepare_recording();

}
void WidRecoding::on_bt_save_clicked()
{
    if(m_state == e_mic_review_ready)
    {

    }
    else if(m_state == e_mic_reviewing)
    {
        //stop the review, i.e. stop the music playing
    }

    //check if the recording can be saved as a valid one
    // Otherwise, messageBox to prompt what to do and stop the page here.

    // if it's saved OK, then
    //switch back to prepar-recording, i.e. mic-off STATE
    prepare_recording();
}
