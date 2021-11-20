#include <QtDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "btnplay.h"


BtnPlay::BtnPlay(const QString &text, BT_type type, QWidget *parent) :
    QPushButton(text, parent),
    m_type(type)
{
    qDebug() << __FUNCTION__ << text;

    m_tips_noFileSet = "No sound file is set !";
    m_style_normal = QString::fromUtf8("background-color:rgb(84, 207, 252) ;\n"
                                       "color: rgb(60, 60, 160);\n");
    m_style_SoundNotSet= QString::fromUtf8("background-color:rgb(84, 207, 252) ;\n"
                                           "color: rgb(220, 220, 220);\n");

    static QFont font;
    font.setFamily(QString::fromUtf8("Arial"));
    font.setPointSize(9);
    font.setBold(false);
    static QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    static QSize size = QSize(120, 90);

    setStyleSheet(m_style_normal);
    setFont(font);
    setSizePolicy(sizePolicy);
    setMaximumSize(size);

    connect(this, SIGNAL(clicked()), this, SLOT(on_clicked()));
    connect(this, SIGNAL(played(BtnPlay*)), parent, SLOT(on_played(BtnPlay*)));
    connect(this, SIGNAL(stopped(BtnPlay*)), parent, SLOT(on_stopped(BtnPlay*)));

    if(is_warning())
    {
        //QPixmap pixmap(":/main/resource/wav.ico");
        //setMask(QRegion(0,0,70,70,QRegion::Ellipse));
        setIcon(QIcon(":/main/resource/wav.ico"));
        setIconSize(QSize(50,50));
        set_soundFile(nullptr);
    }
    else if(is_tone())
    {
        setIcon(QIcon(":/main/resource/tone.ico"));
        setIconSize(QSize(40,40));
        set_soundFile(nullptr);
    }
    else if(is_warningNA() || is_toneNA())
    {
        setText("");

        QLabel *icon_lable = new QLabel();
        QString img;
        if(is_warningNA()){
            img = ":/main/resource/wav-other.ico";
        }
        else {
            img = ":/main/resource/tone-other.ico";
        }
        QPixmap iconMap(img);
        QPixmap iconReshape = iconMap.scaled(QSize(50, 50));
        icon_lable->setPixmap(iconReshape);
        //icon_lable->setFixedSize(40, 40);

        m_text_lable = new QLabel("OTHER\n  ...");
        m_text_lable->setFont(font);
        m_text_lable->setStyleSheet(m_style_normal);

        QHBoxLayout *hLay = new QHBoxLayout();
        hLay->addWidget(icon_lable, 1);
        hLay->addWidget(m_text_lable, 1, Qt::AlignLeft);
        hLay->setSpacing(0);
        hLay->setContentsMargins(0,0,0,0);

        m_NAfileBox = new QComboBox();
        m_NAfileBox->setFont(font);
        //m_NAfileBox->addItems({"1.wav", "2.pcm"});
        connect(m_NAfileBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(on_NAFileBox_IndexChanged(int)));

        QVBoxLayout *vlay = new QVBoxLayout();
        vlay->setSpacing(0);
        vlay->setContentsMargins(5,0,5,5);
        vlay->addLayout(hLay, 1);
        vlay->addWidget(m_NAfileBox, 1);
        //set layout
        setLayout(vlay);
        setTips_soundFileNA();
    }

    m_pb = nullptr;
    m_is_played = false;
    m_soundFile = nullptr;

}

void BtnPlay::on_clicked()
{
    qDebug() << "BtnPlay:" <<__FUNCTION__;

    if(m_pb == nullptr)
    {

        m_pb = new QProgressBar(this);
        m_pb->setWindowFlags(Qt::FramelessWindowHint);
        m_pb->setAttribute(Qt::WA_TranslucentBackground);
        m_pb->setGeometry(0,this->height()-5,this->width()-1, 5);
        m_pb->setTextVisible(false);
        m_pb->setValue(50);
        m_pb->show();
    }
    if (!m_is_played)
    {
        m_is_played = true;
        m_pb->show();
        m_pb->setValue(50);
        emit played(this);
    }
    else
    {
        m_is_played = false;
        m_pb->hide();
        emit stopped(this);
    }
}

BtnPlay::~BtnPlay()
{
    qDebug(__FUNCTION__);
}

void BtnPlay::set_soundFile(const FileTableWidgetItem *fItem)
{
    m_soundFile = fItem;
    if(fItem != nullptr)
    {
        const QFile *sf = fItem->soundFile();
        setToolTip(sf->fileName());
        setStyleSheet(m_style_normal);
    }
    else
    {
        setToolTip(m_tips_noFileSet);
        setStyleSheet(m_style_SoundNotSet);

        //qDebug() << __FUNCTION__ << "No file set !";

    }
}


void BtnPlay::add_soundFileNA(const FileTableWidgetItem *fItem)
{
    if(m_NAfileBox != nullptr)
    {
        m_NAfileBox->addItem(fItem->text());
        m_NAfileList.append(fItem);
    }
}

void BtnPlay::clear_soundFileNA()
{
    Q_ASSERT(m_NAfileBox != nullptr);
    disconnect(m_NAfileBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(on_NAFileBox_IndexChanged(int)));

    //clear box
    if(m_NAfileBox != nullptr)
    {
        int count = m_NAfileBox->count();
        setToolTip(m_tips_noFileSet); //no tips for NA button
        for (int i=0; i<count; i++)
        {
            m_NAfileBox->removeItem(0);
        }
    }
    //clear filelist
    m_NAfileList.clear();
    connect(m_NAfileBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(on_NAFileBox_IndexChanged(int)));

}

void BtnPlay::setTips_soundFileNA()
{
    if(m_NAfileBox != nullptr)
    {
        setToolTip(m_tips_noFileSet);
        int idx = m_NAfileBox->currentIndex();

        if(idx >=0 && idx < m_NAfileList.size())
        {
            const FileTableWidgetItem *fitem = (const FileTableWidgetItem *)m_NAfileList[idx];
            setToolTip(fitem->soundFile()->fileName());
            setStyleSheet(m_style_normal);
            m_text_lable->setStyleSheet(m_style_normal);
        }
        else
        {
            setStyleSheet(m_style_SoundNotSet);
            m_text_lable->setStyleSheet(m_style_SoundNotSet);
        }
    }
}

void BtnPlay::on_NAFileBox_IndexChanged(int index)
{
    qDebug() << __func__ << index;
    setTips_soundFileNA();

}
