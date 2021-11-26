#ifndef HSETTINGS_H
#define HSETTINGS_H

#include <QSettings>
#include <QString>
#include <QByteArray>
#include <QObject>

class HSettings : public QObject
{
    Q_OBJECT
public:
    explicit HSettings(QObject *parent = nullptr);
    void save(){m_settings->sync();}

    typedef enum
    {
        e_enableMode_auto = 0,
        e_enableMode_manual = 1
    }E_EnableMode;

    typedef enum
    {
        e_module_disabled = 0,
        e_module_enabled =1,
    }E_EnableValue;

    void display();

    E_EnableValue str2Val_enableValue(const QString &str) const
        {if (str ==  "enabled") return e_module_enabled;
         else if(str == "disabled") return e_module_disabled;
         else return e_module_enabled;
        }
    const QString val2Str_enableValue(E_EnableValue val) const
    {
        if(val == e_module_disabled) return "disabled";
        else if (val == e_module_enabled) return "enabled";
        else return "enabled";
    }

    E_EnableMode str2Val_enableMode(const QString &str) const
        { if(str == "auto") return e_enableMode_auto;
          else if(str == "manual") return e_enableMode_manual;
          else return e_enableMode_auto;
        }
    const QString val2Str_enableMode(E_EnableMode val) const
    {
        if(val == e_enableMode_auto) return "auto";
        else if(val == e_enableMode_manual) return "manual";
        else return "auto";
    }

    //
    // Local
    //
    QString local_ip()const
        {return m_settings->value("Local/ip", "192.168.30.1").toString();}
    QString local_port()const
        {return m_settings->value("Local/port", 20481).toString();}


    void set_local_ip(const QString &val)
        { m_settings->setValue("Local/ip", val);}
    void set_local_port(int val)
        {m_settings->setValue("Local/port", val);}

    //
    // Remote
    //
    QString remoteKey(int id, const QString &key)const
        {return QString("Remote-%1/").arg(id) + key;}
    QString remote_ip(int id) const
        {if(id == 1) return m_settings->value(remoteKey(id, "ip"), "192.168.30.115").toString();
         else if(id == 2) return m_settings->value(remoteKey(id, "ip"), "192.168.30.116").toString();
         else return "";
        }
    QString remote_port(int id) const
        {return m_settings->value(remoteKey(id, "port"), 20481).toString();}
    QString remote_enableMode(int id) const
        {return m_settings->value(remoteKey(id, "module-enablement"), "auto").toString();}
    QString remote_powerMod1(int id) const
        {return m_settings->value(remoteKey(id, "powerMod1"), "enabled").toString();}
    QString remote_powerMod2(int id) const
        {return m_settings->value(remoteKey(id, "powerMod2"), "enabled").toString();}
    QString remote_powerAmp1(int id) const
        {return m_settings->value(remoteKey(id, "powerAmp1"), "enabled").toString();}
    QString remote_powerAmp2(int id) const
        {return m_settings->value(remoteKey(id, "powerAmp2"), "enabled").toString();}
    int remote_volume(int id)const
        {return m_settings->value(remoteKey(id, "volume"), 7).toInt();}

    //
    //str to val
    //

    E_EnableMode remote_enableMode_v(int id) const
        {return str2Val_enableMode(
                    m_settings->value(remoteKey(id, "module-enablement"), "auto").toString());}
    E_EnableValue remote_powerMod1_v(int id) const
        {return str2Val_enableValue(
                m_settings->value(remoteKey(id, "powerMod1"), "enabled").toString());}
    E_EnableValue remote_powerMod2_v(int id) const
        {return str2Val_enableValue(
                    m_settings->value(remoteKey(id, "powerMod2"), "enabled").toString());}
    E_EnableValue remote_powerAmp1_v(int id) const
        {return str2Val_enableValue(
                    m_settings->value(remoteKey(id, "powerAmp1"), "enabled").toString());}
    E_EnableValue remote_powerAmp2_v(int id) const
        {return str2Val_enableValue(
                    m_settings->value(remoteKey(id, "powerAmp2"), "enabled").toString());}



    void set_remote_ip(int id, const QString &val)
        {m_settings->setValue(remoteKey(id, "ip"), val);}
    void set_remote_port(int id, int val)
        {m_settings->setValue(remoteKey(id, "port"), val);}
    void set_remote_enableMode(int id, const QString &val)
        {m_settings->setValue(remoteKey(id, "module-enablement"), val);}
    void set_remote_powerMod1(int id, const QString &val)
        {m_settings->setValue(remoteKey(id, "powerMod1"), val);}
    void set_remote_powerMod2(int id, const QString &val)
        {m_settings->setValue(remoteKey(id, "powerMod2"), val);}
    void set_remote_powerAmp1(int id, const QString &val)
        {m_settings->setValue(remoteKey(id, "powerAmp1"), val);}
    void set_remote_powerAmp2(int id, const QString &val)
        {m_settings->setValue(remoteKey(id, "powerAmp2"), val);}
    void set_remote_volume(int id, int val) const
        {m_settings->setValue(remoteKey(id, "volume"), val);}



signals:


private:
    QString m_cfg_filename;
    QSettings *m_settings;


};

extern HSettings *g_settings;
#endif // HSETTINGS_H
