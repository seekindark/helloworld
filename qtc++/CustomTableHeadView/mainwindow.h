#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QCheckBox>
#include <QComboBox>

class CustomHeaderView:public QHeaderView
{
    Q_OBJECT

public:
    // Ĭ��ˮƽ����
    CustomHeaderView(Qt::Orientation ori = Qt::Horizontal, QWidget*parent = 0);

    //�Զ���ͷ������Ҫʵ���������
protected:
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const;

public slots:
    void slt_checkbox_click(bool);
    void slt_combox_item_click(QString);

private:
    QCheckBox *m_checkbox;
    QComboBox *m_combox;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QTableView *m_tabView;

};

#endif // MAINWINDOW_H
