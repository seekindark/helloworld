#ifndef BUTTONEDIT_H
#define BUTTONEDIT_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QProgressBar>
#include <QTableWidgetItem>

typedef enum{
    e_edit_play,
    e_edit_delete
}Edit_type;

class ButtonEdit : public QPushButton
{
    Q_OBJECT
public:
    explicit ButtonEdit(Edit_type type, QWidget *parent = nullptr);
    void set_tableItem(const QTableWidgetItem *item) {m_table_item = item;}


signals:
    void bt_play_clicked(const QTableWidgetItem *item, bool is_playing);
    void bt_delete_clicked(const QTableWidgetItem *item);

public slots:
    void on_clicked();


private:
    Edit_type m_type;

    //for play
    bool m_is_playing;
    QProgressBar *m_pb;

    const QTableWidgetItem *m_table_item;

};

#endif // BUTTONEDIT_H
