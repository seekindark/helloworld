#include "hsecurity.h"
#include "mylog.h"
#include <QDebug>
#include "QTextStream"

HSecurity *HSecurity::m_instance = nullptr;
HSecurity::HSecurity()
{
    m_default_user = "administrator";

    m_sam = new QFile("config\\key.sam");
    m_salt = "";
    m_hash = "";

    read_sam();
}
HSecurity::~HSecurity()
{
    qDebug() << __FUNCTION__;
    delete m_sam;

}


void HSecurity::read_sam()
{
    if( m_sam == nullptr || !m_sam->exists())
    {
        MyLog("Warning: key.sam does not exists. System cannot login.");
    }
    else
    {
        m_sam->open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream st(m_sam);
        st.setCodec("UTF-8");
        m_salt = st.readLine();
        m_hash = st.readLine();
        m_sam->close();
    }
}

void HSecurity::save_sam()
{
    if( m_sam == nullptr)
    {
        m_sam = new QFile("config\\key.sam");
    }
    else
    {
        m_sam->open(QIODevice::ReadWrite | QIODevice::Text);
        QTextStream st(m_sam);
        st.setCodec("UTF-8");
        st << m_salt << "\n";
        st << m_hash << "\n";
        m_sam->close();
    }
}

bool HSecurity::validate_pwd(const QString &pwd)
{
    QByteArray pwd_byteArray = combine(m_salt, pwd).toLatin1();
    QByteArray hash_byteArray512 = QCryptographicHash::hash(pwd_byteArray, QCryptographicHash::Sha512);
    return (hash_byteArray512.toBase64() == m_hash);
}
bool HSecurity::validate_user(const QString &user)
{
    return(m_default_user == user);
}

void HSecurity::change_pwd(const QString &pwd)
{
    quint64 salt = QDateTime::currentDateTime().toMSecsSinceEpoch();
    QString salt_str = QString::number(salt, 16);
    QString comb_str = combine(salt_str, pwd);

    qDebug()<< "Change_pwd: pwd, salt, comb=" << pwd << salt_str << comb_str;

    QByteArray pwd_byteArray = comb_str.toLatin1(); //to ascii
    QByteArray hash_byteArray512 = QCryptographicHash::hash(pwd_byteArray, QCryptographicHash::Sha512);
    m_hash = hash_byteArray512.toBase64();
    m_salt = salt_str;
    save_sam();
}
QString HSecurity::combine(const QString & salt, const QString &pwd)
{
    int size = salt.size();
    int u = m_default_user.size();
    int p = pwd.size();
    return (salt.right(size/2) +
            m_default_user.right(u/2) +
            pwd.right(p/2) +
            pwd.left(p/2) +
            m_default_user.left(u/2) +
            salt.left(size/2));
}
