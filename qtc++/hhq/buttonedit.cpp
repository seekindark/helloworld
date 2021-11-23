#include <QPushButton>
#include "buttonedit.h"
#include "haudioproc.h"

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
        //stop the orignnal
        g_audioProc->ed_review_stop();

        //signal to parent table, to remove the row!
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

    }
}

void ButtonEdit::start()
{
    qDebug() << "ButtonEdit::" << __FUNCTION__;
    setIcon(QIcon(":/main/resource/reviewstop.ico"));
    setIconSize(QSize(32,32));
    m_pb->show();
    m_is_playing = true;

    HAudioProc::E_PlayOutResult ret =  g_audioProc->ed_review_start(this, m_pb);
    if(ret == HAudioProc::e_playout_fileNotExist ||
       ret == HAudioProc::e_playout_noFileName)
    {
        QMessageBox::information(this,
                             tr("Edit Recording"),
                             QString("The sound file is not existing on the disk \n"
                                     "or removed after it's inserted here !"),
                             tr("Yes"));
    }
}

void ButtonEdit::stop()
{
    qDebug() << "ButtonEdit::" << __FUNCTION__;
    setIcon(QIcon(":/main/resource/reviewplay.ico"));
    setIconSize(QSize(32,32));
    m_pb->hide();
    m_is_playing = false;

    g_audioProc->ed_review_stop();
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

 void ButtonEdit::set_pb_valaule(int val)
 {
     if(m_pb != nullptr)
     {
        m_pb->setValue(val);
     }
 }
 void ButtonEdit::set_pb_range(int min, int max)
 {
     if(m_pb != nullptr)
     {
         m_pb->setRange(min, max);
     }
 }

int ButtonEdit::pb_value()
{
    if(m_pb != nullptr)
    {
        return m_pb->value();
    }
    return 0;
}

 FileTableWidgetItem * ButtonEdit::fileWidItem_wr()
 {
     const FileTableWidgetItem *fileItem = dynamic_cast<const FileTableWidgetItem *>(m_FileItem);
     return const_cast<FileTableWidgetItem *>(fileItem);
 }
