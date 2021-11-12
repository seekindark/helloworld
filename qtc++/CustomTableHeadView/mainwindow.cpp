#include <QDebug>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setFixedSize(800,480);

    m_tabView = new QTableView(this);
    m_tabView->setFixedSize(800,480);
    QStandardItemModel *model = new QStandardItemModel();
    model->setColumnCount(6);
    m_tabView->setModel(model);

    CustomHeaderView *head = new CustomHeaderView(Qt::Horizontal,this);
    head->setSectionResizeMode(QHeaderView::Fixed);
    m_tabView->setHorizontalHeader(head);
}

MainWindow::~MainWindow()
{

}

CustomHeaderView::CustomHeaderView(Qt::Orientation ori, QWidget *parent)
    :QHeaderView(ori,parent)
{
    m_checkbox = new QCheckBox(this);
    m_combox = new QComboBox(this);

    m_combox->addItem("item1");
    m_combox->addItem("item2");

    connect(m_checkbox,SIGNAL(clicked(bool)),this, SLOT(slt_checkbox_click(bool)));
    connect(m_combox, SIGNAL(currentIndexChanged(QString)),this,SLOT(slt_combox_item_click(QString)));
}

void CustomHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    QHeaderView::paintSection(painter,rect,logicalIndex);

    //logicalIndex 当前第几列，也可以自定义显示其他控件；
    if(logicalIndex == 0)
    {
        QRect tmp;
        tmp.setSize(QSize(20,20));
        tmp.moveCenter(rect.center());
        m_checkbox->setGeometry(tmp);
    }
    else if(logicalIndex ==2)
    {
        m_combox->setGeometry(rect);
    }
}

void CustomHeaderView::slt_checkbox_click(bool state)
{
    qDebug() << "slt_checkbox_click" << state;
    //此处可以自定义信号
}

void CustomHeaderView::slt_combox_item_click(QString itemstr)
{
    qDebug() << "slt_combox_item_click" << itemstr;
    //此处可以自定义信号
}
