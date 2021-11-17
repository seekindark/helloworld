#include <QtDebug>
#include <QProgressBar>
#include "btnplay.h"

BtnPlay::BtnPlay(const QString &text, BT_type type, QWidget *parent) :
    QPushButton(text, parent),
    m_type(type)
{
    qDebug() << __FUNCTION__ << text;

    static QFont font;
    font.setFamily(QString::fromUtf8("Arial"));
    font.setPointSize(9);
    font.setBold(false);
    static QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    static QSize size = QSize(120, 90);

    setStyleSheet(QString::fromUtf8("background-color:rgb(84, 207, 252) ;\n"
                                                 "color: rgb(60, 60, 160);\n"));
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
    }
    else if(is_tone())
    {
        setIcon(QIcon(":/main/resource/tone.ico"));
        setIconSize(QSize(40,40));
    }

    m_pb = nullptr;
    m_is_played = false;
}

void BtnPlay::on_clicked()
{
    qDebug() << "BtnPlay:" <<__FUNCTION__;

    if(m_pb == nullptr)
    {
        m_pb = new QProgressBar(this);
        m_pb->setWindowFlags(Qt::FramelessWindowHint);
        m_pb->setAttribute(Qt::WA_TranslucentBackground);
        m_pb->setGeometry(0,this->height()-5,this->width()-2, 5);
        m_pb->setTextVisible(false);
        m_pb->setValue(50);
        m_pb->show();
    }
    if (!m_is_played)
    {
        m_is_played = true;
        m_pb->show();
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
