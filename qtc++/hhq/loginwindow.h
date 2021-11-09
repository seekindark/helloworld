#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QObject>
#include <QWidget>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QDialog>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

class LoginWindow : public QDialog
{
    Q_OBJECT
public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();
    void keyPressEvent(QKeyEvent *event);

signals:
    void login_OK(const QString &user_name);

public slots:
   void on_login_clicked();
   void on_password_clicked();

private:
   QWidget *m_wm;

   QLabel *m_lab_title;

   QHBoxLayout *m_hLayout_user;
   QLineEdit *m_user;
   QLabel *m_lab_user;

   QHBoxLayout *m_hLayout_pwd;
   QLineEdit *m_password;
   QLabel *m_lab_password;

   QPushButton *m_bt_login;
   QLabel *m_lab_terms;

   QVBoxLayout *m_vlayout;


   void load_img(QLabel *lab, QString filename);

};

#endif // LOGINWINDOW_H
