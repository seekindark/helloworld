#include <QtDebug>
#include <QKeyEvent>
#include <QMessageBox>
#include "loginwindow.h"
#include "mylog.h"

LoginWindow::LoginWindow(QWidget *parent) : QDialog(parent)
{
    m_wm = parent;
    setParent(parent);
    setModal(true);

    //set positon
    resize(350, 450);
    int x = (m_wm->width() - width()) / 2;
    int y = (m_wm->height() - height()) / 2/2;
    move(x, y);

    //background color and shape with 4 arc angles
    setStyleSheet(QString::fromUtf8("background-color:rgb(60, 60, 160) ;\n"
                                    "color: white;\n")); //"border-radius:32px;\n"
    setWindowFlags(Qt::FramelessWindowHint);
    //setAttribute(Qt::WA_TranslucentBackground);

    // Label title
    m_lab_title = new QLabel("IHSS-UDE-800", this);
    m_lab_title->setAlignment(Qt::AlignHCenter);
    QFont font = m_wm->font();
    font.setPointSize(26);
    font.setBold(true);
    //lab_title->setStyleSheet(QString::fromUtf8("color: white;\n"));
    m_lab_title->setFont(font);

    //labe_user
    m_hLayout_user = new QHBoxLayout();
    font.setPointSize(15);
    font.setBold(false);
    m_lab_user = new QLabel(this);
    //m_lab_user->setAlignment(Qt::AlignRight);
    load_img(m_lab_user, ":/login/resource/user.png");
    m_lab_user->setFont(font);
    m_user = new QLineEdit(this);
    m_user->setPlaceholderText("Username");
    m_user->setFont(font);
    m_user->setStyleSheet(QString::fromUtf8("background-color: white;\n"
                                         "color: black")); //"border-radius:8px"
    m_hLayout_user->addWidget(m_lab_user, 1);
    m_hLayout_user->addWidget(m_user, 12);

    //password
    QHBoxLayout *m_hLayout_pwd = new QHBoxLayout();
    m_lab_password = new QLabel(this);
    load_img(m_lab_password, ":/login/resource/key.png");
    //m_lab_password->setAlignment(Qt::AlignRight);
    m_lab_password->setFont(font);
    m_password = new QLineEdit(this);
    m_password->setFont(font);
    m_password->setStyleSheet(QString::fromUtf8("background-color: white;\n"
                                             "color: black")); //"border-radius:8px"
    m_password->setPlaceholderText("Password");
    m_password->setEchoMode(QLineEdit::Password);
    m_password->setContextMenuPolicy(Qt::NoContextMenu);
    m_hLayout_pwd->addWidget(m_lab_password, 1);
    m_hLayout_pwd->addWidget(m_password, 12);
    //Login button
    m_bt_login = new QPushButton("Login", this);
    m_bt_login->setFont(font);
    m_bt_login->setStyleSheet(QString::fromUtf8("background-color: rgb(87, 165, 186);\n"));
    connect(m_bt_login, SIGNAL(clicked()), this, SLOT(on_login_clicked()));
    /*
    "border-radius:8px\n"
    "QPushButton{image:url("");}"
    "QPushButton:hover{image:url("");}"
    "QPushButton:press{image:url("");}"
    //m_bt_login->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;");
    */

    //label terms
    m_lab_terms = new QLabel("By using this system you are agreeing to the terms and conditions.", this);
    m_lab_terms->setWordWrap(true);
    font.setPointSize(9);
    m_lab_terms->setFont(font);
    //m_lab_terms->setStyleSheet(QString::fromUtf8("color: white;\n"));

    //Layout
    m_vlayout = new QVBoxLayout(this);
    m_vlayout->setContentsMargins(25, 80, 20, 25);
    m_vlayout->setSpacing(30);
    m_vlayout->addWidget(m_lab_title, 3);
    m_vlayout->addLayout(m_hLayout_user, 1);
    m_vlayout->addLayout(m_hLayout_pwd, 1);
    m_vlayout->addWidget(m_bt_login, 1);
    m_vlayout->addWidget(m_lab_terms, 2);

    setLayout(m_vlayout);
    m_user->setFocus(); //first focus should be on user input

    hide();

}

LoginWindow::~LoginWindow()
{
    qDebug() << __FUNCTION__;
}


void LoginWindow::load_img(QLabel *lab, QString filename)
{
    if (lab != nullptr && filename.length() > 0)
    {
        QImage* img = new QImage();
        img->load(filename);
        QImage* imgScaled = new QImage();
        *imgScaled=img->scaled(lab->width(),
                               lab->height(),
                               Qt::KeepAspectRatio);
        lab->setPixmap(QPixmap::fromImage(*imgScaled));
    }
}

void LoginWindow::on_login_clicked()
{
    if (true)
    //if(m_user->text() == "admin" && m_password->text() == "123")
    {
        MyLog(QString("Login successfully with %1").arg(m_user->text()));
        this->close();
        emit login_OK(m_user->text());
    }
    else
    {
        QMessageBox::warning(this, tr("Error"),
                             tr("Username or password is not correct !"),
                             QMessageBox::Yes);
        MyLog(QString("Failed to login with %1").arg(m_user->text()));

    }
}


void LoginWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        if (m_user->hasFocus())
        {
            m_password->setFocus();
        }
        else if(m_password->hasFocus())
        {
            m_bt_login->setFocus();
            emit m_bt_login->clicked();
        }

    }
}

void LoginWindow::on_password_clicked()
{
    qDebug() << __func__;

}
