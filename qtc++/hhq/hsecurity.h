#ifndef HSECURITY_H
#define HSECURITY_H

#include <QCryptographicHash>
#include <QtGlobal>
#include <QRandomGenerator>
#include <QDateTime>
#include <QString>
#include <QFile>

class HSecurity
{
public:
    HSecurity();
    ~HSecurity();

    bool validate_pwd(const QString &pwd);
    bool validate_user(const QString &user);
    void change_pwd(const QString &pwd);

    static HSecurity * get_instance()
    {
        if(m_instance == nullptr)
        {
            m_instance = new HSecurity();
        }
        return m_instance;
    }

private:
    QString m_default_user;
    QString m_hash;
    QString m_salt;
    QFile *m_sam;
    static HSecurity *m_instance;

    void save_sam();
    void read_sam();
    QString combine(const QString & salt, const QString &pwd);
};

#endif // HSECURITY_H
