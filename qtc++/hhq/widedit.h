#ifndef WIDEDIT_H
#define WIDEDIT_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QMenu>
#include <QFile>
#include <QComboBox>
#include "buttonedit.h"
#include "filetablewidgetitem.h"


class WidEdit : public QWidget
{
    Q_OBJECT
public:
    explicit WidEdit(QWidget *parent = nullptr);

    void insert_row(int row, const QStringList &list_orig);
    void read_config(const QString &filename);
    void write_config(const QString &filename);
    void write_config_default(){write_config(m_config_file);}

    const QString config_dir(){return m_config_dir;}

    FileTableWidgetItemList_R read_fileTabel_List();

signals:

public slots:
    void on_bt_delete_clicked(const QTableWidgetItem *item);
    void on_bt_play_clicked(const QTableWidgetItem *item, bool is_playing);
    //void on_itemChanged(QTreeWidgetItem * treeItem, int column);
    void on_RepeatStateChanged(int flag);
    void on_GroupChanged(int index);
    void on_SoundAliasChanged(int index);
    void on_tableCell_Changed(int row, int column);
    void on_tableCell_Clicked(int row, int column); // used for debug only !!

    void on_AddFile_clicked();

private:
    QString m_config_dir;
    QString m_config_file;
    QStringList m_headline;
    int m_columnCount;
    QStringList m_WarningAlias;
    QStringList m_ToneAlias;
    QStringList m_repeatTypes;
    QStringList m_soundGroup;

    QLabel *m_lab_title;
    QTableWidget *m_table;
    QPushButton *m_bt_addFile;


    QVBoxLayout *m_layout_all;
    QMenu *m_menu;

    //playing mutex
    QTableWidgetItem *m_itemPlaying;


    int warningAlias_Index(const QString &str);
    int warningAlias_NA_index(){return m_WarningAlias.size()-1;}
    bool repeat_value(const QString &str);
    const QString & soundGroup_str(const QString &typeStr);
    int soundGroup_index(const QString &str);

    bool is_soundGroup_warning(const QString &str) {return str == m_soundGroup[0];}
    bool is_soundGroup_warning(int index){return (index == 0);}
    bool is_NA_alias(int grp, int aliasIdx);

    const QString & soundAlias(int grp, int aliasIdx);
    const QString & soundGroupStr(int idx){
        if (idx <m_soundGroup.size())
            {return m_soundGroup[idx];}
        else
            {return m_soundGroup[0];}
    }
    const QString &repeatStr(int flag){return (flag ? m_repeatTypes[0]:m_repeatTypes[1]);}


    int toneAlias_NA_index(){return m_ToneAlias.size()-1;}
    int toneAlias_index(const QString &str);
    bool check_fileExisting(const QString &name);

    //default params for a warning: warning, NA, No
    const QString & default_group(){return m_soundGroup[0];}
    const QString & default_warningAlias(){return m_WarningAlias[warningAlias_NA_index()];}
    const QString & default_repeat(){return m_repeatTypes[1];}

    //playing mutex
    void update_playingItem(QTableWidgetItem *item);
    inline FileTableWidgetItem * fileWidItem(const QTableWidgetItem *item);
    inline QComboBox *comBox_WR (const QWidget *tabWid);

    void clear_comboBox(QComboBox *cb, int itemCount);

    void set_comboxAliasIndx_wo_signal(FileTableWidgetItem *fileItem_wr,
                                       QComboBox *comboxAlias,
                                       int aliasIndx);
    void changeGroupItems_comboxAliasIndx_wo_signal(FileTableWidgetItem *fileItem_wr,
                                                          QComboBox *comboxAlias);
    void change_tabItem_text_wo_signal(FileTableWidgetItem *fileItem_wr,
                                                const QString *tips,
                                                const QString *text);
    bool find_file(const QString  &filepath);
    QStringList make_a_sound_w_defult_param(const QString &filepath);
};

#endif // WIDEDIT_H
