#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QRegExpValidator>
#include "hsecurity.h"
#include "widchangepwd.h"


WidChangePwd::WidChangePwd(QWidget *parent) : QDialog(parent)
{
    QFont font = this->font();
    font.setFamily("Arial");
    font.setPointSize(12);
    font.setBold(false);
    setFont(font);

    setFixedSize(300, 150);
    setWindowTitle("Change Password");
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);

    QString  LE_style = "background-color: white";
    QString Label_style = "color:white";

    m_formLayout = new QFormLayout(this);

    m_pwd_tips = "The effective password should \n"
                 "1. contain 8 to 16 characters and\n"
                 "2. contain at least two types of digits, uppercase\n"
                 "   and lowercase letters, and other symbles like\n"
                 "   #,@,+,=,-,!,& etc.\n"
                 "3. not contain white-space characters.\n";


    m_LE_old_pwd = new QLineEdit(this);
    m_LE_new_pwd = new QLineEdit(this);
    m_LE_new_pwd_again = new QLineEdit(this);

    m_LE_old_pwd->setStyleSheet(LE_style);
    m_LE_new_pwd->setStyleSheet(LE_style);
    m_LE_new_pwd_again->setStyleSheet(LE_style);

    m_LE_new_pwd->setToolTip(m_pwd_tips);
    m_LE_new_pwd_again->setToolTip(m_pwd_tips);

    m_LE_old_pwd->setEchoMode(QLineEdit::Password);
    m_LE_new_pwd->setEchoMode(QLineEdit::Password);
    m_LE_new_pwd_again->setEchoMode(QLineEdit::Password);

    m_LE_old_pwd->setFixedHeight(25);
    m_LE_new_pwd->setFixedHeight(25);
    m_LE_new_pwd_again->setFixedHeight(25);

    m_LE_old_pwd->setFont(font);
    m_LE_new_pwd->setFont(font);
    m_LE_new_pwd_again->setFont(font);

    m_BT_ok = new QPushButton("OK", this);
    m_BT_cancle = new QPushButton ("CANCEL", this);
    m_BT_ok->setFont(font);
    m_BT_ok->setFixedHeight(25);
    m_BT_ok->setStyleSheet(Label_style);
    m_BT_cancle->setFont(font);
    m_BT_cancle->setFixedHeight(25);
    m_BT_cancle->setStyleSheet(Label_style);


    QHBoxLayout *btLay = new QHBoxLayout();
    btLay->addWidget(m_BT_cancle);
    btLay->addWidget(m_BT_ok);

    QLabel *curr_pwd = new QLabel("Current:", this);
    curr_pwd->setFont(font);
    curr_pwd->setFixedHeight(20);
    curr_pwd->setStyleSheet(Label_style);
    QLabel *new_pwd = new QLabel("New:", this);
    new_pwd->setFont(font);
    new_pwd->setFixedHeight(20);
    new_pwd->setStyleSheet(Label_style);
    QLabel *new_pwd2 = new QLabel("New:", this);
    new_pwd2->setFont(font);
    new_pwd2->setFixedHeight(20);
    new_pwd2->setStyleSheet(Label_style);

    m_formLayout->addRow(curr_pwd, m_LE_old_pwd);
    m_formLayout->addRow(new_pwd, m_LE_new_pwd);
    m_formLayout->addRow(new_pwd2, m_LE_new_pwd_again);
    m_formLayout->addRow(btLay);
    m_formLayout->setSpacing(10);

    connect(m_BT_ok, SIGNAL(clicked()), this, SLOT(on_ok_clicked()));
    connect(m_BT_cancle, SIGNAL(clicked()), this, SLOT(reject()));

}
void WidChangePwd::on_ok_clicked()
{
    if( ! HSecurity::get_instance()->validate_pwd(m_LE_old_pwd->text()))
    {
        QMessageBox::information(this,
                                 "Change Password",
                                 "Current password is NOT correct !",
                                 tr("Yes"));
        return;
    }
    else
    {
        if(m_LE_new_pwd_again->text() != m_LE_new_pwd->text())
        {
            QMessageBox::information(this,
                                     "Change Password",
                                     "Two new passwords don't match !",
                                     tr("Yes"));
        }
        else
        {
            if(check_space_chars(m_LE_new_pwd->text()))
            {
                QMessageBox::information(this,
                                         "Change Password",
                                         QString("White-space characters are NOT allowed !"),
                                         tr("Yes"));
                return;
            }

            QString reg = "^(?![\\d]+$)(?![a-zA-Z]+$)(?![^\\da-zA-Z]+$).{8,16}$";
            QRegExp rx(reg);
            int pos = 0;
            QRegExpValidator EV(rx, 0);
            QString pwd_str = m_LE_new_pwd->text();
            int ret = EV.validate(pwd_str, pos);
            if(ret == QValidator::Acceptable)
            {
                qDebug() << __func__ << "password: " << pwd_str << "matches rule";
                QMessageBox::information(this,
                                         "Change Password",
                                         QString("New password is changed successfully !"),
                                         tr("Yes"));
                HSecurity::get_instance()->change_pwd(m_LE_new_pwd->text());

                accept();
            }
            else
            {
                QMessageBox::information(this,
                                         "Change Password",
                                         QString("New passwords don't match the rule below: \n\n") + m_pwd_tips,
                                         tr("Yes"));
                qDebug() << __func__ <<"password invalid from pos="<< pos;
            }
            //set
        }
    }
}


bool WidChangePwd::check_space_chars(const QString &str)
{
    QString reg("^\\w+\\S+$");
    QRegExp rx(reg);
    int pos = 0;
    QRegExpValidator EV(rx, 0);
    QString pwd_str = str;
    int ret = EV.validate(pwd_str, pos);
    if(ret == QValidator::Acceptable)
    {
        return false;
    }
    else
    {
        return true;
    }
}


WidChangePwd::~WidChangePwd()
{
    qDebug() << __FUNCTION__;
}
