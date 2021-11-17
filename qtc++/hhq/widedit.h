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
#include "buttonedit.h"


class FileTableWidgetItem: public QTableWidgetItem
{
    friend class WidEdit;
public:
    explicit FileTableWidgetItem(const QString &text, int type = QTableWidgetItem::UserType);
    ~FileTableWidgetItem();

    void set_soundFile(QFile *qf) {m_soundFile = qf;}
    QFile *soundFile(){return m_soundFile;}
private:
    QFile *m_soundFile;
};


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

signals:

public slots:
    void on_bt_delete_clicked(const QTableWidgetItem *item);
    void on_bt_play_clicked(const QTableWidgetItem *item, bool is_playing);
    //void on_itemChanged(QTreeWidgetItem * treeItem, int column);
    void on_RepeatStateChanged(int flag);
    void on_WarningTypeChanged(int index);
    void on_tableCell_Changed(int row, int cloumn);

private:
    QString m_config_dir;
    QString m_config_file;
    QStringList m_headline;
    int m_columnCount;
    QStringList m_WarningTypes;
    QStringList m_repeatTypes;
    QStringList m_soundTypes;

    QLabel *m_lab_title;
    QTableWidget *m_table;


    QVBoxLayout *m_layout_all;
    QMenu *m_menu;

    //playing mutex
    QTableWidgetItem *m_itemPlaying;


    int warningType_Index(const QString &str);
    bool repeat_value(const QString &str);
    const QString & sound_type(const QString &typeStr);
    bool is_sound_warning(const QString &str) {return str == m_soundTypes[0];}
    int warningType_NA_index(){return m_WarningTypes.size()-1;}

    //playing mutex
    void update_playingItem(QTableWidgetItem *item);

};

#endif // WIDEDIT_H
