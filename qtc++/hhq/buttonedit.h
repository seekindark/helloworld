#ifndef BUTTONEDIT_H
#define BUTTONEDIT_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QProgressBar>
#include <QTableWidgetItem>
#include "filetablewidgetitem.h"

typedef enum{
    e_edit_play,
    e_edit_delete
}Edit_type;

class ButtonEdit : public QPushButton
{
    Q_OBJECT
public:
    explicit ButtonEdit(Edit_type type, QWidget *parent = nullptr);
    ~ButtonEdit();

    void set_fileItem(const QTableWidgetItem *item) {m_FileItem = item;}
    FileTableWidgetItem * fileWidItem_wr();

    inline void set_pb_valaule(int val);
    inline void set_pb_range(int min, int max);
    inline int pb_value();
    void set_progress(int val);

    void start();
    void stop();


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

    const QTableWidgetItem *m_FileItem;

};

#endif // BUTTONEDIT_H
