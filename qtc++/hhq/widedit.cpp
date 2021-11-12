
#include <QtDebug>
#include <QMenu>
#include <QHeaderView>
#include <QComboBox>
#include <QCheckBox>
#include "widedit.h"

WidEdit::WidEdit(QWidget *parent) : QWidget(parent)
{
    QFont font;
    font.setFamily(QString::fromUtf8("Arial"));
    font.setPointSize(13);
    font.setBold(false);

    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    //QSize size = QSize(120, 90);
    QString style = QString::fromUtf8("background-color:rgb(84, 207, 252) ;\n"
                                      "color: rgb(60, 60, 160);\n");
    //
    // title
    //
    m_lab_title = new QLabel("EDIT SOUND", this);
    m_lab_title->setMaximumHeight(30);
    m_lab_title->setStyleSheet(QString::fromUtf8("background-color:rgb(84, 207, 252) ;\n"
                                                 "color: rgb(60, 60, 160);\n"));
    m_lab_title->setAlignment(Qt::AlignLeft |Qt:: AlignVCenter);
    m_lab_title->setFont(font);
    m_lab_title->setSizePolicy(sizePolicy);

    //
    //  table
    //

    m_table = new QTableWidget(this);
    m_table->setColumnCount(5);     // 5 colums settings
    m_table->setHorizontalHeaderLabels({"File", "Type", "Warning", "Repeat", "Action"});
    m_table->horizontalHeader()->setFont(font);
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table->setSortingEnabled(true);
    m_table->horizontalHeader()->setVisible(true);
    m_table->verticalHeader()->setVisible(false);
    m_table->setWordWrap(true);
    m_table->setFont(font);
    m_table->setStyleSheet(style);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    //
    //Layout
    //
    m_layout_all = new QVBoxLayout(this);
    m_layout_all->setSpacing(20);
    m_layout_all->setContentsMargins(0, 20, 0, 0);
    m_layout_all->addWidget(m_lab_title, 1);
    m_layout_all->addWidget(m_table, 4);
    QLabel *dummy_lable = new QLabel();
    m_layout_all->addWidget(dummy_lable, 1);

    //
    // construct table rows
    //
    insert_row(m_table->rowCount(), {"1.wav", "Warning", "English", "Yes"});
    insert_row(m_table->rowCount(), {"2.wav", "Warning", "Local", "Yes"});
    insert_row(m_table->rowCount(), {"3.wav", "Warning", "Siren", "Yes"});
    insert_row(m_table->rowCount(), {"4.wav", "Warning", "NA", "No"});
    insert_row(m_table->rowCount(), {"32k.pcm", "Tone", "NA", "Yes"});
    insert_row(m_table->rowCount(), {"16.pcm", "Tone", "NA", "Yes"});
    insert_row(m_table->rowCount(), {"8k.pcm", "Tone", "NA", "Yes"});
    insert_row(m_table->rowCount(), {"8k.pcm", "Tone", "NA", "Yes"});

    insert_row(m_table->rowCount(), {"test.pcm", "Tone", "NA", "No"});
    insert_row(m_table->rowCount(), {"8k.pcm", "Tone", "NA", "Yes"});
    insert_row(m_table->rowCount(), {"8k.pcm", "Tone", "NA", "Yes"});
    insert_row(m_table->rowCount(), {"8k.pcm", "Tone", "NA", "Yes"});
    insert_row(m_table->rowCount(), {"8k.pcm", "Tone", "NA", "Yes"});

    qDebug() << __FUNCTION__ << "table.column, row=" << m_table->columnCount() <<m_table->rowCount();

 }

void WidEdit::insert_row(int row, const QStringList &list)
{
    QFont font;
    font.setFamily(QString::fromUtf8("Arial"));
    font.setPointSize(10);
    font.setBold(false);
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    QString style = QString::fromUtf8("background-color:rgb(84, 207, 252) ;\n"
                                      "color: rgb(60, 60, 160);\n");

    qDebug() << __FUNCTION__<<" rown=" <<row;
    int rowCount = row;
    m_table->insertRow(rowCount);
    for(int i = 0; i < 5; i++)
    {
        if(i == 0)
        {
            QTableWidgetItem *item = new QTableWidgetItem(list[i]);
            item->setFont(font);
            m_table->setItem(rowCount,i,item);
        }
        else if(i == 1)
        {
            QTableWidgetItem *item = new QTableWidgetItem("Warning");
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            item->setFont(font);
            m_table->setItem(rowCount,i,item);
            //item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
        else if(i==2)
        {

            QComboBox *comboBox = new QComboBox();
            comboBox->addItems({"English", "Local", "Siren", "NA"});
            comboBox->setCurrentIndex(1);
            comboBox->setStyleSheet(style);
            comboBox->setFont(font);
            m_table->setCellWidget(rowCount,i,comboBox);


        }
        else if(i==3)
        {
            QCheckBox *checkBox = new QCheckBox("Repeat");
            checkBox->setCheckState(Qt::Checked);
            checkBox->setStyleSheet(style);
            checkBox->setFont(font);
            m_table->setCellWidget(rowCount,i,checkBox);
        }
        else if(i == 4)
        {
            QWidget *action = new QWidget();
            ButtonEdit *bt_del = new ButtonEdit(e_edit_delete, action);
            ButtonEdit *bt_play = new ButtonEdit(e_edit_play, action);
            bt_del->setIcon(QIcon(":/main/resource/trash.ico"));
            bt_del->setIconSize(QSize(20,20));
            bt_play->setIcon(QIcon(":/main/resource/reviewplay.ico"));
            bt_play->setIconSize(QSize(32,32));
            bt_del->setSizePolicy(sizePolicy);
            bt_play->setSizePolicy(sizePolicy);
            bt_del->setMaximumHeight(30);
            bt_play->setMaximumHeight(30);
            QHBoxLayout *layout = new QHBoxLayout(action);
            layout->addWidget(bt_play);
            layout->addWidget(bt_del);
            layout->setContentsMargins(5, 0, 5, 0);
            m_table->setCellWidget(rowCount,i,action);
            bt_del->set_tableItem(m_table->item(rowCount, 0)); // used for callback, only first item can be used here
            bt_play->set_tableItem(m_table->item(rowCount, 0)); //used for callback, only first item can be used here

            connect(bt_del, SIGNAL(bt_delete_clicked(const QTableWidgetItem*)),
                    this, SLOT(on_bt_delete_clicked(const QTableWidgetItem*)));
            connect(bt_play, SIGNAL(bt_play_clicked(const QTableWidgetItem*,bool)),
                    this, SLOT(on_bt_play_clicked(const QTableWidgetItem*,bool)));
        }

    }

}

void WidEdit::on_bt_delete_clicked(const QTableWidgetItem * item)
{
    int row = m_table->row(item);
    m_table->removeRow(row);;

    qDebug("%s deleted row %d", __FUNCTION__, row);
    //qDebug() << m_table->item(row, 0)->text();
}
void WidEdit::on_bt_play_clicked(const QTableWidgetItem *item, bool is_playing)
{
    int row = m_table->row(item);
    qDebug("%s playing row %d, is_playing=%d", __FUNCTION__, row, is_playing);
    qDebug() << m_table->item(row, 0)->text();
}

