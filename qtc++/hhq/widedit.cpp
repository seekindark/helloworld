
#include <QtDebug>
#include <QMenu>
#include <QHeaderView>
#include <QComboBox>
#include <QCheckBox>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QFileDialog>
#include "mylog.h"
#include "widedit.h"
#include <QList>




WidEdit::WidEdit(QWidget *parent) : QWidget(parent)
{
    qDebug() << __FUNCTION__;

    m_config_dir = "config";
    m_config_file = m_config_dir + "\\" + QString::fromUtf8("sound.ini");
    m_WarningAlias = QStringList({"English", "Local", "Siren", "NA"});
    m_soundGroup = QStringList({"Warning", "Tone"});
    m_ToneAlias = QStringList({"Tone-1", "Tone-2","Tone-3", "Tone-4","Tone-5", "Tone-6", "NA"});
    m_repeatTypes = QStringList({"Yes", "No"});
    m_headline = QStringList({"File", "Group", "Alias", "Repeat", "Action"});
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

    //Button Add Sound
    QLabel *lab_bt_addFile = new QLabel("");
    lab_bt_addFile->setMaximumHeight(35);
    lab_bt_addFile->setStyleSheet(QString::fromUtf8("background-color:rgb(84, 207, 252) ;\n"
                                                 "color: rgb(60, 60, 160);\n"));//
    lab_bt_addFile->setAlignment(Qt::AlignLeft |Qt:: AlignVCenter);
    lab_bt_addFile->setFont(font);
    lab_bt_addFile->setSizePolicy(sizePolicy);

    m_bt_addFile = new QPushButton("Add Sound ...", lab_bt_addFile);
    m_bt_addFile->setMaximumWidth(150);
    m_bt_addFile->setMaximumHeight(30);
    m_bt_addFile->setSizePolicy(sizePolicy);
    m_bt_addFile->setStyleSheet(QString::fromUtf8("background-color: rgb(87, 165, 186 );\n"
                                                  "font:13pt 'Arial';\n"
                                                  "color: rgb(60, 60, 160)\n")); //(43, 153, 240)
    QHBoxLayout *hBtLayout = new QHBoxLayout();
    hBtLayout->setAlignment(Qt::AlignRight);
    hBtLayout->setContentsMargins(0,0,5,0);
    hBtLayout->addWidget(m_bt_addFile);
    lab_bt_addFile->setLayout(hBtLayout);
    connect(m_bt_addFile, SIGNAL(clicked()), this, SLOT(on_AddFile_clicked()));

    //
    //Layout
    //
    m_layout_all = new QVBoxLayout(this);
    m_layout_all->setSpacing(20);
    m_layout_all->setContentsMargins(0, 20, 0, 20);
    m_layout_all->addWidget(m_lab_title, 1);
    m_layout_all->addWidget(m_table, 4);
    m_layout_all->addWidget(lab_bt_addFile, 1);

    //
    // read table rows
    //
    read_config(m_config_file);
    // we must connt sig and slot after load the config.
    // or it will trigger many information messageBox
    connect(m_table, SIGNAL(cellChanged(int,int)),
            this, SLOT(on_tableCell_Changed(int,int)));
    connect(m_table, SIGNAL(cellClicked(int,int)),
            this, SLOT(on_tableCell_Clicked(int,int)));

    qDebug() << __FUNCTION__ << "table.column, row=" << m_table->columnCount() <<m_table->rowCount();

 }
int WidEdit::warningAlias_Index(const QString &str)
{
    int idx = m_WarningAlias.indexOf(str);
    if (idx == -1)
        return warningAlias_NA_index();
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

const QString & WidEdit::soundGroup_str(const QString &typeStr)
{
    if(typeStr.size() >= 2)
    {
        if((QString::compare(typeStr.left(2), "WA", Qt::CaseInsensitive)) == 0)
        {
            return m_soundGroup[0];
        }
        else if((QString::compare(typeStr.left(2), "TO", Qt::CaseInsensitive)) == 0)
        {
            return m_soundGroup[1];
        }
        else
        {
            //Default warning
            return m_soundGroup[0];
        }
    }
    else
    {
        // default Warning
        return m_soundGroup[0];
    }
}

int WidEdit::soundGroup_index(const QString &str)
{
    int idx = m_soundGroup.indexOf(str);
    if(idx == -1)
    {
        return 0; // Warning group
    }
    else
    {
        return idx;
    }
}
int WidEdit::toneAlias_index(const QString &str)
{
    int idx = m_ToneAlias.indexOf(str);
    if(idx == -1)
    {
        return toneAlias_NA_index(); // Warning group
    }
    else
    {
        return idx;
    }
}

 bool WidEdit::is_NA_alias(int grp, int aliasIdx)
 {
    if (is_soundGroup_warning(grp))
    {
        return (aliasIdx == warningAlias_NA_index());
    }
    else
    {
        return (aliasIdx == toneAlias_NA_index());
    }
 }

 const QString & WidEdit::soundAlias(int grp, int aliasIdx)
 {
    if (is_soundGroup_warning(grp))
    {
        return m_WarningAlias[aliasIdx];
    }
    else
    {
        return m_ToneAlias[aliasIdx];
    }
 }

bool WidEdit::check_fileExisting(const QString &name)
{
    return (QFile(name.trimmed()).exists());

}

//
//  Note: Before insert to talbe, the file-exisitng is already checked !!
//
void WidEdit::insert_row(int row, const QStringList &list_orig)
{
    static QFont font;
    font.setFamily(QString::fromUtf8("Arial"));
    font.setPointSize(10);
    font.setBold(false);
    static QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    static QString style = QString::fromUtf8("background-color:rgb(84, 207, 252) ;\n"
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
    FileTableWidgetItem *fileItem = nullptr;
    for(int i = 0; i < m_columnCount; i++)
    {
        if(i == 0) // file
        {
            //list_trimed[i] is already the full-name = path + name
            QFile *soundFile = new QFile(list_trimed[i]);
            QFileInfo fileInfo(*soundFile);
            // only add a new row after checking the file exists !!!
            m_table->insertRow(rowCount);
            fileItem = new FileTableWidgetItem(fileInfo.fileName()); // fill only filename and excluding the path part
            fileItem->set_soundFile(soundFile);
            fileItem->setFont(font);
            fileItem->setToolTip(fileInfo.absoluteFilePath()); // show fullname in tips
            m_table->setItem(rowCount,i,fileItem);
            MyLog(QString("Read %1 OK").arg(list_trimed[i]));

        }
        else if(i == 1) //group
        {
            QComboBox *comboBox = new QComboBox();
            comboBox->addItems(m_soundGroup);
            int groupIdx = soundGroup_index(soundGroup_str(list_trimed[i]));
            fileItem->set_groupIdx(groupIdx);
            comboBox->setCurrentIndex(groupIdx);
            comboBox->setStyleSheet(style);
            comboBox->setFont(font);
            m_table->setCellWidget(rowCount,i,comboBox);
            //sig-slot must be set after all setCurrentIndex()
            connect(comboBox, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(on_GroupChanged(int)));
        }
        else if(i == 2) // alias
        {
            QComboBox *comboBox = new QComboBox();

            if(is_soundGroup_warning(soundGroup_str(list_trimed[1])))
            {
                int aliasIndex = warningAlias_Index(list_trimed[i]);
                comboBox->addItems(m_WarningAlias);
                comboBox->setCurrentIndex(aliasIndex);
                fileItem->set_aliasIdx(aliasIndex);
            }
            else
            {
                int aliasIndex = toneAlias_index(list_trimed[i]);
                comboBox->addItems(m_ToneAlias);
                comboBox->setCurrentIndex(aliasIndex);
                fileItem->set_aliasIdx(aliasIndex);
            }
            comboBox->setStyleSheet(style);
            comboBox->setFont(font);
            m_table->setCellWidget(rowCount,i,comboBox);
            //sig-slot must be set after all setCurrentIndex()
            connect(comboBox, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(on_SoundAliasChanged(int)));

        }
        else if(i == 3) // repeat
        {
            QCheckBox *checkBox = new QCheckBox("Repeat");
            bool val =repeat_value(list_trimed[i]);
            fileItem->set_repeat(val);
            if(val)
                checkBox->setCheckState(Qt::Checked);
            else
                checkBox->setCheckState(Qt::Unchecked);

            checkBox->setStyleSheet(style);
            checkBox->setFont(font);
            m_table->setCellWidget(rowCount,i,checkBox);
            connect(checkBox, SIGNAL(stateChanged(int)), this, SLOT(on_RepeatStateChanged(int)));
        }
        else if(i == 4) //actions
        {
            QWidget *action = new QWidget();
            ButtonEdit *bt_del = nullptr;
            //bool is_warning = is_soundGroup_warning(soundGroup_str(list_trimed[1]));
            //qDebug() << "typestr=" << list_trimed[1] << "is_warning = "<< is_warning;
            //both tone and warning need 'delete' action for now
            //if(is_warning)
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
            //if(is_warning)
            {
                layout->addWidget(bt_del);
            }
            layout->setContentsMargins(5, 0, 5, 0);
            m_table->setCellWidget(rowCount,i,action);

            //callback by signal+slot
            //if(is_warning)
            {
                bt_del->set_fileItem(m_table->item(rowCount, 0)); // used for callback, only first item can be used here
                connect(bt_del, SIGNAL(bt_delete_clicked(const QTableWidgetItem*)),
                        this, SLOT(on_bt_delete_clicked(const QTableWidgetItem*)));
            }
            bt_play->set_fileItem(m_table->item(rowCount, 0)); //used for callback, only first item can be used here
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
    QRegExp rx("[\t]+");      // 分隔符不支持空格，是为了文件名里可以有空格，那么在取得独立单词后需要trim一下空白字符
    lineList = lineStr.split(rx);
    qDebug() << "LineList: " << lineList;
    while(!output.atEnd())
    {
        lineStr = output.readLine();
        lineList = lineStr.split(rx); //去除换行符获取字符串，存放到表中
        qDebug() << "line: " << lineList;

        //check if the file is existing
        if(check_fileExisting(lineList[0]))
        {
            insert_row(m_table->rowCount(), lineList);
        }
        else
        {
            qDebug() << __func__ << lineList[0] << "not exists";
            MyLog(QString("Read config file:%1 NOT exists and removed !").arg(lineList[0]));
        }
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
            FileTableWidgetItem * fileItem = fileWidItem(m_table->item(i, 0));
            QFileInfo fi(*(fileItem->soundFile()));
            const QComboBox *comboBox_grp = dynamic_cast<const QComboBox *>(m_table->cellWidget(i, 1));
            const QComboBox *comboBox_alias = dynamic_cast<const QComboBox *>(m_table->cellWidget(i, 2));
            const QCheckBox *checkBox_repeat = dynamic_cast<const QCheckBox *>(m_table->cellWidget(i, 3));

            line += fi.absoluteFilePath();line += "\t\t";
            line += comboBox_grp->currentText();line += "\t\t";
            line += comboBox_alias->currentText();line += "\t\t";
            line += checkBox_repeat->checkState()?"Yes":"No";line += "\n";
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
        FileTableWidgetItem *fileItem_wr = fileWidItem(item);
        MyLog(QString("%1 is removed").arg(fileItem_wr->text()));

        fileItem_wr->remove_file();
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
    MyLog(QString("%1 is %2").arg(m_table->item(row, 0)->text(), is_playing?"playing":"stopped"));

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
    qDebug() << __FUNCTION__ << "Repeat is changed to " <<flag;

    int currentRow = m_table->currentRow();
    FileTableWidgetItem *fileItem_wr = fileWidItem(m_table->item(currentRow, 0));
    qDebug() << __FUNCTION__ << fileItem_wr->text() << ": repeat is changed to " << flag;

    fileItem_wr->set_repeat(flag);

    MyLog(QString("%1 is set to repeat %2").arg(fileItem_wr->text(), flag?"True":"False"));

    write_config_default();
}


void WidEdit::on_GroupChanged(int index)
{
    qDebug() << __FUNCTION__ << index;

    int currentRow = m_table->currentRow();
    FileTableWidgetItem *fileItem_wr = fileWidItem(m_table->item(currentRow, 0));
    qDebug() << __FUNCTION__ << fileItem_wr->text() << ": group is changed to " << m_soundGroup[index];
    fileItem_wr->set_groupIdx(index);

    QComboBox *comboBox_alias = comBox_WR(m_table->cellWidget(currentRow, 2));

    changeGroupItems_comboxAliasIndx_wo_signal(fileItem_wr, comboBox_alias);

    MyLog(QString("%1 is set to repeat %2").arg(fileItem_wr->text(), m_soundGroup[index]));

    write_config_default();

}

void WidEdit::on_SoundAliasChanged(int index)
{
    if(index >= 0 )
    {
        qDebug() << __FUNCTION__ << "index " << index ;
    }
    else
    {
        //ignore clear acion
        qDebug() << __FUNCTION__ << "index " << index << "Skipped clear-actions !";
        return;
    }
    /*
     * 4 kinds of warning types: {"English", "Local", "Siren", "NA"}
     * and only 'NA', i.e. index last can be multiple existing, and the others must be unique.
     */

    int currentRow = m_table->currentRow();
    FileTableWidgetItem *fileItem_wr = fileWidItem(m_table->item(currentRow, 0));

    qDebug() << __FUNCTION__ << fileItem_wr->text() << ": sound alias is changed to index  "
             <<index << soundAlias(fileItem_wr->groupIdx(), index);

    if(is_NA_alias(fileItem_wr->groupIdx(), index))
    {
        //
        //it's always OK to change type to 'NA'
        //
        fileItem_wr->set_aliasIdx(index);
        MyLog(QString("Sound file %1, warning alias is changed to NA").
                    arg(fileItem_wr->text()));
    }
    else   // non NA
    {
        fileItem_wr->set_aliasIdx(index);
        MyLog(QString("Sound file %1, alias is changed to %2").
                    arg(fileItem_wr->text(), soundAlias(fileItem_wr->groupIdx(), index)));
        //
        //warningAlias or ToneAlias mutex check
        //
        for(int i=0; i < m_table->rowCount(); i++)
        {
            if (i!=currentRow) // skip current row
            {
                FileTableWidgetItem *fileItem_wr_temp = fileWidItem(m_table->item(i, 0));
                QComboBox *comboBox_alias = comBox_WR(m_table->cellWidget(i, 2));

                if(fileItem_wr->groupIdx() == fileItem_wr_temp->groupIdx()
                    &&
                    comboBox_alias->currentIndex() == index)
                {
                    int aliasIndex = is_soundGroup_warning(fileItem_wr->groupIdx()) ?
                                        warningAlias_NA_index():toneAlias_NA_index();
                    set_comboxAliasIndx_wo_signal(fileItem_wr_temp,  comboBox_alias, aliasIndex);
                }
            }
        }

    }

    // save change to sound.ini
    write_config_default();
}

void WidEdit::changeGroupItems_comboxAliasIndx_wo_signal(FileTableWidgetItem *fileItem_wr,
                                                      QComboBox *comboxAlias)
{
    qDebug() << __FUNCTION__;

    //firstly dis sig and slot
    disconnect(comboxAlias, SIGNAL(currentIndexChanged(int)),
            this, SLOT(on_SoundAliasChanged(int)));

    //then update the items inside the comboBox
    clear_comboBox(comboxAlias, comboxAlias->count());
    if(is_soundGroup_warning(fileItem_wr->groupIdx())) // warning group
    {
        comboxAlias->addItems(m_WarningAlias);
        comboxAlias->setCurrentIndex(warningAlias_NA_index());
    }
    else
    {
        comboxAlias->addItems(m_ToneAlias);
        comboxAlias->setCurrentIndex(toneAlias_NA_index());
    }
    //udpate alias index
    fileItem_wr->set_aliasIdx(comboxAlias->currentIndex());
    //then re-conn sig and slot
    connect(comboxAlias, SIGNAL(currentIndexChanged(int)),
            this, SLOT(on_SoundAliasChanged(int)));

    MyLog(QString("Sound file %1, alias is changed to NA").
                arg(fileItem_wr->text()));

}

//used in slot process: on_SoundAliasChanged only !!!
void WidEdit::set_comboxAliasIndx_wo_signal(FileTableWidgetItem *fileItem_wr,
                                             QComboBox *comboxAlias,
                                             int aliasIndx)
{
    qDebug(__FUNCTION__);
    Q_ASSERT(comboxAlias != nullptr);

    disconnect(comboxAlias, SIGNAL(currentIndexChanged(int)),
            this, SLOT(on_SoundAliasChanged(int)));

    QMessageBox::information(this,
                             tr("Change Alias"),
                             QString("Sound file: %1 has duplicated alias:%2 "
                                     "with the selected file within the same group,"
                                     " and thus is changed to NA").
                                arg(fileItem_wr->text(), comboxAlias->currentText()),
                             tr("Yes"));

    fileItem_wr->set_aliasIdx(aliasIndx);
    comboxAlias->setCurrentIndex(aliasIndx);

    qDebug() << __FUNCTION__ << fileItem_wr->text() << ": warning alias is changed to NA";
    MyLog(QString("Sound file %1, alias is changed to %2").
                arg(fileItem_wr->text(), soundAlias(fileItem_wr->groupIdx(), aliasIndx)));

    connect(comboxAlias, SIGNAL(currentIndexChanged(int)),
            this, SLOT(on_SoundAliasChanged(int)));
}

//used in slots only
void WidEdit::change_tabItem_text_wo_signal(FileTableWidgetItem *fileItem_wr,
                                            const QString *tips,
                                            const QString *text)
{
    qDebug(__FUNCTION__);

    Q_ASSERT(fileItem_wr != nullptr);
    disconnect(m_table, SIGNAL(cellChanged(int,int)),
            this, SLOT(on_tableCell_Changed(int,int)));
    if(tips != nullptr)
    {
        fileItem_wr->setToolTip(*tips);
    }
    if(text != nullptr)
    {
        fileItem_wr->setText(*text);
    }
    connect(m_table, SIGNAL(cellChanged(int,int)),
            this, SLOT(on_tableCell_Changed(int,int)));

}

void WidEdit::on_tableCell_Changed(int row, int column)
{
    qDebug() << __FUNCTION__ << "row"<<row << "column" << column;
    if (column == 0)
    {
        //only 1st column supports to edit (i.e. edit file name)
        FileTableWidgetItem *fileItem_wr = fileWidItem(m_table->item(row, 0));
        QFileInfo fileinfo(*fileItem_wr->soundFile());
        //old filename
        QString old_filename = fileinfo.fileName();
        QString new_full_filename = fileinfo.absolutePath();
                new_full_filename += "/";
                new_full_filename += fileItem_wr->text();

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
            change_tabItem_text_wo_signal(fileItem_wr, nullptr, &old_filename);

        }
        else
        {
            qDebug() << __FUNCTION__ << old_filename << "is successfully changed to"
                     <<fileItem_wr->soundFile()->fileName();
            MyLog(QString("%1 is successfully renamed to %2").
                  arg(old_filename, fileItem_wr->soundFile()->fileName()));

            //update tips as well
            QString tips = QFileInfo(*fileItem_wr->soundFile()).absoluteFilePath();
            change_tabItem_text_wo_signal(fileItem_wr,
                                          &tips,
                                          nullptr);

            //store the new change
            write_config_default();

        }

    }
}

void WidEdit::on_tableCell_Clicked(int row, int column)
{
    qDebug() << __FUNCTION__;

    if(column == 0)
    {
        FileTableWidgetItem *fileItem_wr = fileWidItem(m_table->item(row, 0));
        QFile *soundFile = fileItem_wr->soundFile();
        QFileInfo fi(*soundFile);
        qDebug() << "FileTableItem SoundFile: \n"
                 << "name="<<soundFile->fileName() <<"\n"
                 << "Group="<<soundGroupStr(fileItem_wr->groupIdx())<<"\n"
                 << "Alias="<<soundAlias(fileItem_wr->groupIdx(), fileItem_wr->aliasIdx())<<"\n"
                 << "Repeat="<<repeatStr( fileItem_wr->isRepeat())<<"\n"
                 << "QFileInfo: filepath=" <<fi.filePath()<<"\n"
                 << "          filename=" << fi.fileName()<<"\n"
                 << "Tips:" <<fileItem_wr->toolTip()<<"\n";
    }

}


void WidEdit::on_AddFile_clicked()
{
    QStringList fileList = QFileDialog::getOpenFileNames(this, "Add sound files",
                                                        m_config_dir,
                                                         "*.wav *.pcm");
    qDebug() << __FUNCTION__ << fileList;

    for(int i=0; i<fileList.size(); i++)
    {
        if(! find_file(fileList[i]))
        {

            insert_row(m_table->rowCount(),
                       make_a_sound_w_defult_param(fileList[i]));
            MyLog(QString("Added file %1").arg(fileList[i]));
            qDebug() << __FUNCTION__ << "Added file OK: " << fileList[i];
        }
        else
        {
            //it's already added in, just ignore it
            MyLog(QString("Skip file %1 as it's already in").arg(fileList[i]));
            qDebug() << __FUNCTION__ << "Skip duplicated file : " << fileList[i];
        }
    }

    write_config_default();

}

QStringList WidEdit::make_a_sound_w_defult_param(const QString &filepath)
{
    QStringList list;
    list.append(filepath);
    list.append(default_group());
    list.append(default_warningAlias());
    list.append(default_repeat());
    return list;
}

bool WidEdit::find_file(const QString  &filepath)
{
    for(int i=0; i < m_table->rowCount(); i++)
    {
        FileTableWidgetItem * fileItem_wr = fileWidItem(m_table->item(i, 0));
        const QString tab_filepath = QFileInfo(*fileItem_wr->soundFile()).filePath();
        if(tab_filepath == filepath)
        {
            return true;
        }
    }

    return false;
}


FileTableWidgetItem * WidEdit::fileWidItem(const QTableWidgetItem *item)
{
    const FileTableWidgetItem *fileItem = dynamic_cast<const FileTableWidgetItem *>(item);
    return const_cast<FileTableWidgetItem *>(fileItem);
}

QComboBox * WidEdit::comBox_WR (const QWidget *tabWid)
{
    const QComboBox *comBox = dynamic_cast<const QComboBox *>(tabWid);
    return const_cast<QComboBox *>(comBox);
}
void WidEdit::clear_comboBox(QComboBox *cb, int itemCount)
{
    for (int i=0; i<itemCount; i++)
    {
        // alwasy remove 1st one, as the index of each item will be changed accordingly after it's removed
        qDebug() << __FUNCTION__ << "remove" << i << ":" << cb->itemText(0);
        cb->removeItem(0);
    }
}

FileTableWidgetItemList_R WidEdit::read_fileTabel_List()
{
    FileTableWidgetItemList_R list;
    if(m_table == nullptr)
    {
        return list;
    }
    for (int i=0; i<m_table->rowCount(); i++)
    {
        FileTableWidgetItem * fileItem_wr = fileWidItem(m_table->item(i, 0));
        list.append(fileItem_wr);
    }

    return list;
}

void WidEdit::on_SaveRecording(const QStringList &rf)
{
    insert_row(m_table->rowCount(), rf);
}
