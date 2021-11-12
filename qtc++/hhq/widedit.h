#ifndef WIDEDIT_H
#define WIDEDIT_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QMenu>
#include "buttonedit.h"

class WidEdit : public QWidget
{
    Q_OBJECT
public:
    explicit WidEdit(QWidget *parent = nullptr);

    void insert_row(int row, const QStringList &list);

signals:

public slots:
    void on_bt_delete_clicked(const QTableWidgetItem *item);
    void on_bt_play_clicked(const QTableWidgetItem *item, bool is_playing);
    //void on_itemChanged(QTreeWidgetItem * treeItem, int column);
private:

    QLabel *m_lab_title;
    QTableWidget *m_table;


    QVBoxLayout *m_layout_all;
    QMenu *m_menu;

};

#endif // WIDEDIT_H
