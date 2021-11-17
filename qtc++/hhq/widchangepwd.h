#ifndef WIDCHANGEPWD_H
#define WIDCHANGEPWD_H

#include <QObject>
#include <QDialog>
#include <QFormLayout>
#include <QPushButton>
#include <QLineEdit>

class WidChangePwd : public QDialog
{
    Q_OBJECT
public:
    explicit WidChangePwd(QWidget *parent = nullptr);
    ~WidChangePwd();



signals:

public slots:
    void on_ok_clicked();

private:
    QFormLayout *m_formLayout;
    QLineEdit *m_LE_old_pwd;
    QLineEdit *m_LE_new_pwd;
    QLineEdit *m_LE_new_pwd_again;
    QPushButton *m_BT_ok;
    QPushButton *m_BT_cancle;

    QString m_pwd_tips;

    bool check_space_chars(const QString &str);

};

#endif // WIDCHANGEPWD_H
