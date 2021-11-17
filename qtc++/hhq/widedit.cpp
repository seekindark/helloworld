
#include <QtDebug>
#include <QMenu>
#include <QHeaderView>
#include <QComboBox>
#include <QCheckBox>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include "mylog.h"
#include "widedit.h"



WidEdit::WidEdit(QWidget *parent) : QWidget(parent)
{
    qDebug() << __FUNCTION__;

    m_config_dir = "config";
    m_config_file = m_config_dir + "\\" + QString::fromUtf8("config.ini");
    m_WarningTypes = QStringList({"English", "Local", "Siren", "NA"});
    m_soundTypes = QStringList({"Warning", "Tone"});
    m_repeatTypes = QStringList({"Yes", "No"});
    m_headline = QStringList({"File", "Type", "Warning", "Repeat", "Action"});
    m_columnCount = m_headline.size();
    m_itemPlaying = nullptr;



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
    m_table->setColumnCount(m_columnCount);     // 5 colums settings
    m_table->setHorizontalHeaderLabels(m_headline);
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
    // read table rows
    //
    read_config(m_config_file);
    // we must connt sig and slot after load the config.
    // or it will trigger many information messageBox
    connect(m_table, SIGNAL(cellChanged(int,int)),
            this, SLOT(on_tableCell_Changed(int,int)));

    qDebug() << __FUNCTION__ << "table.column, row=" << m_table->columnCount() <<m_table->rowCount();

 }
int WidEdit::warningType_Index(const QString &str)
{
    int idx = m_WarningTypes.indexOf(str);
    if (idx == -1)
        return warningType_NA_index();
    else
        return idx;
}

bool WidEdit::repeat_value(const QString &str)
{
    if(m_repeatTypes.indexOf(str) == 0)
        return true;
    else
        return false;
}

const QString & WidEdit::sound_type(const QString &typeStr)
{
    if(typeStr.size() >= 2)
    {
        if((QString::compare(typeStr.left(2), "WA", Qt::CaseInsensitive)) == 0)
        {
            return m_soundTypes[0];
        }
        else if((QString::compare(typeStr.left(2), "TO", Qt::CaseInsensitive)) == 0)
        {
            return m_soundTypes[1];
        }
        else
        {
            //Default warning
            return m_soundTypes[0];
        }
    }
    else
    {
        // default Warning
        return m_soundTypes[0];
    }
}

void WidEdit::insert_row(int row, const QStringList &list_orig)
{
    QFont font;
    font.setFamily(QString::fromUtf8("Arial"));
    font.setPointSize(10);
    font.setBold(false);
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    QString style = QString::fromUtf8("background-color:rgb(84, 207, 252) ;\n"
                                      "color: rgb(60, 60, 160);\n");

    QStringList list_trimed;
    foreach (QString str, list_orig)
    {
        list_trimed.append(str.trimmed());
    }
    qDebug() << __FUNCTION__ <<"trimmed list = " << list_trimed;
    qDebug() << __FUNCTION__<<" rown=" <<row;
    if(list_trimed[0].isEmpty())
    {
        qDebug() << __FUNCTION__<<" empty file name of this row, discard it";
        return;
    }

    int rowCount = row;
    for(int i = 0; i < m_columnCount; i++)
    {
        if(i == 0)
        {
            QString full_filename = m_config_dir + "\\" + list_trimed[i];
            QFile *soundFile = new QFile(full_filename);
            if(! soundFile->exists())
            {
                qDebug() << full_filename << "not exists, and skip this line !";
                MyLog(QString("%1 does not exist, and skip it !"));
                continue;
            }
            // only add a new row after checking the file exists !!!
            m_table->insertRow(rowCount);
            FileTableWidgetItem *item = new FileTableWidgetItem(list_trimed[i]);
            item->set_soundFile(soundFile);
            item->setFont(font);
            m_table->setItem(rowCount,i,item);
            MyLog(QString("Read %1 OK").arg(list_trimed[i]));

        }
        else if(i == 1)
        {
            QTableWidgetItem *item = new QTableWidgetItem(sound_type(list_trimed[i]));
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            item->setFont(font);
            m_table->setItem(rowCount,i,item);
            //item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
        else if(i==2)
        {
            QComboBox *comboBox = new QComboBox();
            comboBox->addItems(m_WarningTypes);
            comboBox->setCurrentIndex(warningType_Index(list_trimed[i]));
            comboBox->setStyleSheet(style);
            comboBox->setFont(font);
            m_table->setCellWidget(rowCount,i,comboBox);
            if(!is_sound_warning(sound_type(list_trimed[1])))
            {
                //if it's Tone, we should set this combobox non-editable
                comboBox->setCurrentIndex(warningType_NA_index());
                comboBox->setEnabled(false);
            }
            //sig-slot must be set after all setCurrentIndex()
            connect(comboBox, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(on_WarningTypeChanged(int)));

        }
        else if(i==3)
        {
            QCheckBox *checkBox = new QCheckBox("Repeat");
            if(repeat_value(list_trimed[i]))
                checkBox->setCheckState(Qt::Checked);
            else
                checkBox->setCheckState(Qt::Unchecked);
            checkBox->setStyleSheet(style);
            checkBox->setFont(font);
            m_table->setCellWidget(rowCount,i,checkBox);
            connect(checkBox, SIGNAL(stateChanged(int)), this, SLOT(on_RepeatStateChanged(int)));
        }
        else if(i == 4)
        {
            QWidget *action = new QWidget();
            ButtonEdit *bt_del = nullptr;
            bool is_warning = is_sound_warning(sound_type(list_trimed[1]));
            qDebug() << "typestr=" << list_trimed[1] << "is_warning = "<< is_warning;
            if(is_warning)
            {
                bt_del = new ButtonEdit(e_edit_delete, action);
                bt_del->setObjectName("bt_del");
                bt_del->setIcon(QIcon(":/main/resource/trash.ico"));
                bt_del->setIconSize(QSize(20,20));
                bt_del->setSizePolicy(sizePolicy);
                bt_del->setMaximumHeight(30);
            }

            ButtonEdit *bt_play = new ButtonEdit(e_edit_play, action);
            bt_play->setObjectName("bt_play");
            bt_play->setIcon(QIcon(":/main/resource/reviewplay.ico"));
            bt_play->setIconSize(QSize(32,32));
            bt_play->setSizePolicy(sizePolicy);
            bt_play->setMaximumHeight(30);

            QHBoxLayout *layout = new QHBoxLayout(action);
            layout->setObjectName("action_layout");
            layout->addWidget(bt_play);
            if(is_warning)
            {
                layout->addWidget(bt_del);
            }
            layout->setContentsMargins(5, 0, 5, 0);
            m_table->setCellWidget(rowCount,i,action);

            //callback by signal+slot
            if(is_warning)
            {
                bt_del->set_tableItem(m_table->item(rowCount, 0)); // used for callback, only first item can be used here
                connect(bt_del, SIGNAL(bt_delete_clicked(const QTableWidgetItem*)),
                        this, SLOT(on_bt_delete_clicked(const QTableWidgetItem*)));
            }
            bt_play->set_tableItem(m_table->item(rowCount, 0)); //used for callback, only first item can be used here
            connect(bt_play, SIGNAL(bt_play_clicked(const QTableWidgetItem*,bool)),
                    this, SLOT(on_bt_play_clicked(const QTableWidgetItem*,bool)));
        }

    }

}

void WidEdit::read_config(const QString &config)
{
    qDebug()<< ">> " << __FUNCTION__;

    //从文件读取数据
    Q_ASSERT(m_table != nullptr);

    if(!QFile::exists(config))
    {
        MyLog(QString("Error: config file:%1 does not exist!").arg(config));
        return;
    }
    QFile f(config);
    f.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream output(&f);
    output.setCodec("UTF-8");
    //QHash<QString, QString> varHash;
    QString lineStr;        //文件的每一行的字符串
    QStringList lineList;   //整行字符串，分割处理为单个字符串，存入到表中
    //varHash.clear();
    lineList.clear();       //操作前，清空
    //
    //go through the file
    //
    // read out the 1st line, i.e. the head and skip it.
    // Note: It's not checked for now !!
    lineStr = output.readLine(); //读取文件的一行
    QRegExp rx("[\t,;]+");      // 分隔符不支持空格，是为了文件名里可以有空格，那么在取得独立单词后需要trim一下空白字符
    lineList = lineStr.split(rx);
    qDebug() << "LineList: " << lineList;
    while(!output.atEnd())
    {
        lineStr = output.readLine();
        lineList = lineStr.split(rx); //去除换行符获取字符串，存放到表中
        qDebug() << "line: " << lineList;
        insert_row(m_table->rowCount(), lineList);
    }
    f.close();//关闭文件

    MyLog(QString("Read config file:%1 successfully!").arg(config));
    qDebug()<< "<< " << __FUNCTION__;
}


void WidEdit::write_config(const QString &config)
{
    Q_ASSERT(m_table != nullptr);
    qDebug()<< ">> " << __FUNCTION__ << "rowCount=" << m_table->rowCount();

    QFile f(config);
    f.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream input(&f);
    input.setCodec("UTF-8");
    QString line;

    for(int i=-1; i < m_table->rowCount(); i++)
    {
        // the last column 'action' does not need to write in
        line.clear();
        if(i == -1)
        {
            // write head
            line += m_headline[0];line += "\t\t";
            line += m_headline[1];line += "\t\t";
            line += m_headline[2];line += "\t\t";
            line += m_headline[3];line += "\n";
        }
        else
        {
            line += m_table->item(i, 0)->text();line += "\t\t";
            line += m_table->item(i, 1)->text();line += "\t\t";
            const QComboBox *comboBox = dynamic_cast<const QComboBox *>(m_table->cellWidget(i, 2));
            //qDebug("row=%d, comboBox=0x%p", i, comboBox);
            line += comboBox->currentText();line += "\t\t";
            const QCheckBox *checkBox = dynamic_cast<const QCheckBox *>(m_table->cellWidget(i, 3));
            line += checkBox->checkState()?"Yes":"No";line += "\n";
        }
        //qDebug() << "Writing in line: " << line;
        input << line;
    }
    f.close();//关闭文件

    qDebug()<< "<< " << __FUNCTION__;
}


void WidEdit::on_bt_delete_clicked(const QTableWidgetItem * item)
{
    int row = m_table->row(item);

    int ret =  QMessageBox::information( this, tr("Edit Recording"),
                          QString("Do you really want to delete %1 ?").arg(item->text()),
                          tr("Yes"), tr("No"),
                          0, 1 );
    if(ret == 0)
    {
        m_table->removeRow(row);

        if(m_itemPlaying == item)
        {
            //if the row to be deleted is playing, we should update the playingItem as null.
            qDebug("set the playingItem to be null as it's deleted");
            update_playingItem(nullptr);
        }
        qDebug("%s deleted row %d", __FUNCTION__, row);

        write_config_default();
    }
    else
    {
        //do nothing
    }


}
void WidEdit::on_bt_play_clicked(const QTableWidgetItem *item, bool is_playing)
{
    int row = m_table->row(item);
    //set selected the row
    m_table->selectRow(row);

    //stop the other row which is playing
    if(is_playing)
    {
        update_playingItem(const_cast<QTableWidgetItem *>(item));
    }
    else
    {
        update_playingItem(nullptr);
    }

    qDebug("%s playing row %d, is_playing=%d", __FUNCTION__, row, is_playing);
    qDebug() << m_table->item(row, 0)->text();

}

void WidEdit::update_playingItem(QTableWidgetItem *item)
{
    if(m_itemPlaying != nullptr && item != nullptr)
    {
        //stop current playing
        int row = m_table->row(m_itemPlaying);
        QWidget * widItem = m_table->cellWidget(row, 4);
        ButtonEdit *bt_play =  widItem->findChild<ButtonEdit *>("bt_play");

        if(bt_play != nullptr)
        {
            bt_play->stop();
        }
    }

    //update it.
    m_itemPlaying = item;
}


void WidEdit::on_RepeatStateChanged(int flag)
{
    qDebug() << __FUNCTION__ << "on Repeat is changed to " <<flag;

    write_config_default();
}
void WidEdit::on_WarningTypeChanged(int index)
{
    qDebug() << __FUNCTION__ << "index " << m_WarningTypes[index];
    /*
     * 4 kinds of warning types: {"English", "Local", "Siren", "NA"}
     * and only 'NA', i.e. index last can be multiple existing, and the others must be unique.
     */
    //warningType mutex check
    //
    if(index == warningType_NA_index())
    {
        //it's always OK to change type to 'NA'
    }
    else
    {
        int currentRow = m_table->currentRow();
        const QTableWidgetItem *currentItemFilename = m_table->item(currentRow, 0);
        qDebug() << __FUNCTION__ << currentItemFilename->text() << ": warning type is changed to index  "
                 <<index << m_WarningTypes[index];
        for(int i=0; i < m_table->rowCount(); i++)
        {
            if (i!=currentRow) // skip current row
            {
                const QComboBox *comboBox = dynamic_cast<const QComboBox *>(m_table->cellWidget(i, 2));
                // if there's duplicated warning-type, then change the other to 'NA'
                if (comboBox != nullptr && comboBox->currentIndex() == index)
                {
                    const QTableWidgetItem *conflictItemFile = m_table->item(i, 0);
                    QMessageBox::information(this,
                                             tr("Change Warning Type"),
                                             QString("Sound file: %1 has duplicated warning type:%2 with the selected file, and thus is changed to NA").
                                                arg(conflictItemFile->text(), m_WarningTypes[index]),
                                             tr("Yes"));
                    const_cast<QComboBox *>(comboBox)->setCurrentIndex(warningType_NA_index());

                    qDebug() << __FUNCTION__ << conflictItemFile->text() << ": warning type is changed to NA";

                    break;
                }
            }
        }
    }

    // save change to config.ini
    write_config_default();
}


void WidEdit::on_tableCell_Changed(int row, int column)
{
    qDebug() << __FUNCTION__ << "row"<<row << "column" << column;
    if (column == 0)
    {
        //only 1st column supports to edit (i.e. edit file name)
        const QTableWidgetItem *item = m_table->item(row, 0);
        const FileTableWidgetItem *fileItem = dynamic_cast<const FileTableWidgetItem *>(item);
        FileTableWidgetItem *fileItem_wr = const_cast<FileTableWidgetItem *>(fileItem);
        QString new_full_filename = m_config_dir + "\\" + fileItem_wr->text();

        //old filename
        QFileInfo fileinfo(*fileItem_wr->soundFile());
        QString old_filename = fileinfo.fileName();

        if(fileItem_wr->text() == old_filename)
        {
            //do nothing
        }
        else if( ! fileItem_wr->soundFile()->rename(new_full_filename))
        {
            QMessageBox::warning(this,
                                 tr("Change File Name"),
                                 QString("Failed to rename to %1, as it's already existing !").
                                    arg(fileItem_wr->text()),
                                 tr("Yes"));
            qDebug() << __FUNCTION__ << "Failed to rename to " << fileItem_wr->text() << "as it's already existing";
            //set back to the original filename
            fileItem_wr->setText(old_filename);
        }
        else
        {
            qDebug() << __FUNCTION__ << old_filename << "is successfully changed to" <<fileItem_wr->soundFile()->fileName();

            //store the new change
            write_config_default();
        }

    }
}

FileTableWidgetItem::FileTableWidgetItem(const QString &text, int type):QTableWidgetItem(text, type)
{
    m_soundFile = nullptr;
    qDebug() << __FUNCTION__;
}

FileTableWidgetItem::~FileTableWidgetItem()
{
    delete m_soundFile; m_soundFile = nullptr;
    qDebug() << __FUNCTION__;
}
