#ifndef FILETABLEWIDGETITEM_H
#define FILETABLEWIDGETITEM_H

#include <QObject>
#include <QWidget>
#include <QTableWidget>
#include <QFile>

class FileTableWidgetItem : public QTableWidgetItem
{
    friend class WidEdit;
public:

    explicit FileTableWidgetItem(const QString &text, int type = QTableWidgetItem::UserType);
    ~FileTableWidgetItem();

    enum E_SoundGroupIndex
    {
        e_soundGroup_warning = 0,
        e_soundGroup_tone = 1
    };

    enum E_WarningAliasIndex
    {
        e_warning_english = 0,
        e_warning_local = 1,
        e_warning_siren = 2,
        e_warning_NA = 3

    };

    enum E_ToneAliasIndex
    {
        e_tone_1 = 0,
        e_tone_2 = 1,
        e_tone_3 = 2,
        e_tone_4 = 3,
        e_tone_5 = 4,
        e_tone_6 = 5,
        e_tone_NA = 6
    };

    void set_soundFile(QFile *qf) {m_soundFile = qf;}
    QFile *soundFile() const {return m_soundFile;}
    bool remove_file(){return m_soundFile->remove();}

    int groupIdx()const{return m_group;}
    int aliasIdx()const{return m_alias;}
    bool isRepeat()const{return m_repeat;}

    void set_groupIdx(int idx){m_group = idx;}
    void set_aliasIdx(int idx){m_alias = idx;}
    void set_repeat(bool flag){m_repeat = flag;}

    bool is_warnig() const {return (m_group == e_soundGroup_warning);}
    bool is_tone() const{return (m_group == e_soundGroup_tone);}
    bool is_warningNA()const{return (is_warnig() && m_alias == e_warning_NA);}
    bool is_toneNA()const{return (is_tone() && m_alias == e_tone_NA);}
private:
    QFile *m_soundFile;

    int m_group;
    int m_alias;
    bool m_repeat;

};

typedef QList <const FileTableWidgetItem *>  FileTableWidgetItemList_R;
typedef QList <const FileTableWidgetItem *>::iterator  FileTableWidgetItemList_R_itor;
typedef QList <FileTableWidgetItem *>  FileTableWidgetItemList_WR;
typedef QList <FileTableWidgetItem *>::iterator  FileTableWidgetItemList_WR_itor;

#endif // FILETABLEWIDGETITEM_H

