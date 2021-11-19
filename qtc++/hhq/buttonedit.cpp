#include <QPushButton>
#include "buttonedit.h"

ButtonEdit::ButtonEdit(Edit_type type, QWidget *parent) :
    QPushButton(parent)
{
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    m_type = type;
    m_FileItem = nullptr;

    if(type == e_edit_play)
    {
        setIcon(QIcon(":/main/resource/reviewplay.ico"));
        setIconSize(QSize(32,32));

        m_pb = new QProgressBar(this);
        m_pb->setWindowFlags(Qt::FramelessWindowHint);
        m_pb->setAttribute(Qt::WA_TranslucentBackground);
        m_pb->setGeometry(0,this->height()-5,this->width()-2, 5);
        m_pb->setTextVisible(false);
        m_pb->setValue(50);
        m_pb->hide();
        m_is_playing = false;
    }
    else if(type == e_edit_delete)
    {
        setIcon(QIcon(":/main/resource/trash.ico"));
        setIconSize(QSize(20,20));
    }

    setSizePolicy(sizePolicy);
    setMaximumHeight(30);

    connect(this, SIGNAL(clicked()), this, SLOT(on_clicked()));

}

ButtonEdit::~ButtonEdit()
{
    qDebug(__FUNCTION__);
}

void ButtonEdit::on_clicked()
{
    if(m_type == e_edit_delete)
    {
        emit bt_delete_clicked(m_FileItem);
    }
    else if(m_type == e_edit_play)
    {
        if(!m_is_playing)
        {
            start();
        }
        else
        {
            stop();
        }

        emit bt_play_clicked(m_FileItem, m_is_playing);
    }
}

void ButtonEdit::start()
{
    setIcon(QIcon(":/main/resource/reviewstop.ico"));
    setIconSize(QSize(32,32));
    m_pb->show();
    m_pb->setValue(10);
    m_is_playing = true;
}

void ButtonEdit::stop()
{
    setIcon(QIcon(":/main/resource/reviewplay.ico"));
    setIconSize(QSize(32,32));
    m_pb->setValue(0);
    m_pb->hide();
    m_is_playing = false;
}

 void ButtonEdit::set_progress(int val)
 {
    if(m_pb != NULL)
    {
        m_pb->setValue(val);
        if(val == 0)
        {
            stop();
        }
    }
 }
