#include <QApplication> //used for qApp
#include <QWidget>
#include <QDebug>
#include <QDateTime>
#include <QFile>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QMessageBox>
#include <QCloseEvent>
#include "mainwindow.h"
#include "mylog.h"



MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    // fix size, and disable maximize
    resize(900, 700);
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    setFixedSize(width(), height());

    //set background color
    //background color RBG=0X2B99F0 (RGB:43/153/240)
    setStyleSheet(QString::fromUtf8("background-color: rgb(43, 153, 240);\n"));
    //set title and icon
    QFont font = this->font();
    font.setFamily("Arial");
    setFont(font);
    setWindowTitle("IHSS-UDE-800");
    setWindowIcon(QIcon("resource\\D4.ico"));
    //setIconSize(QSize(30, 30));

    //init members
    m_loginWind = new LoginWindow(this);
    QObject::connect(m_loginWind, SIGNAL(login_OK(QString)), this, SLOT(on_login_OK(QString)));

    m_user_name = "NonLogin";
    m_bt_system = nullptr;
    m_lab_user = nullptr;
    m_tab = nullptr;

    construct_main();
}

MainWindow::~MainWindow()
{
    qDebug() << __func__;
}

void MainWindow::construct_main()
{
    qDebug() << ">> " <<__FUNCTION__;

    QFont font = this->font();
    font.setFamily("Arial");
    font.setPointSize(20);
    font.setBold(true);
    setFont(font);

    //title part
    ///
    m_lab_title = new QLabel("      IHSS-UDE-800", this);
    m_lab_title->setGeometry(QRect(0, 0, width(), 50));
    m_lab_title->setStyleSheet(QString::fromUtf8("background-color: rgb(60, 60, 160);\n"
                                        "color: white;\n"));//
    font.setPointSize(18);
    m_lab_title->setFont(font);

    //warning tips: system is very dangeous!

    ///
    m_lab_user = new QLabel(QString("Hello %1").arg(m_user_name), this);
    m_lab_user->setGeometry(QRect(width()/2, 0, width()/4, 50));
    m_lab_user->setStyleSheet(QString::fromUtf8("background-color:rgb(60, 60, 160) ;\n"
                                                 "color: grey;\n"));
    m_lab_user->setAlignment(Qt::AlignRight |Qt:: AlignVCenter);
    font.setPointSize(12);
    font.setBold(false);
    m_lab_user->setFont(font);
    ///
    m_bt_system = new QPushButton("System", this);
    m_bt_system->setStyleSheet(QString::fromUtf8("background-color:rgb(99, 161, 227) ;\n"
                                    "color: white;\n"));
    m_bt_system->setFont(font);
    m_bt_system->setGeometry(width()*3/4+50, 10, 100, 30);

    construct_system_menu();

    QVBoxLayout *m_vLayout_main = new QVBoxLayout(this);
    m_vLayout_main->setContentsMargins(50,50+20,50,0);
    m_tab = new QTabWidget(this);
    font.setPointSize(12);
    //m_tab->setStyleSheet(QString::fromUtf8("background-color:rgb(43, 153, 240) ;\n"
    //                                             "color: grey;\n"));
    //set background and other style
    m_tab->setAttribute(Qt::WA_StyledBackground);
    m_tab->setStyleSheet("QTabBar::tab{background-color:rgb(100, 200, 220);color:rgb(60, 60, 160);font:11pt 'Arial'}\
                         QTabBar::tab{margin-right: 10px; margin-left: 0px; height: 35px; min-width: 100px}\
                         QTabWidget::tab-bar{border-width:0px;}\
                         QTabBar::tab::selected{background-color:rgb(60, 60, 160);color:white;font:12pt 'Arial'; height: 38px}");
                         //QTabWidget::pane{border-style: outset;}  //tab and form的边界线上置

    m_tab->setFont(font);
    m_tab->setDocumentMode(true);

    m_wid_dashboard = new WidDashboard(m_tab);
    m_wid_livecall = new WidBroadCast(m_tab);
    m_wid_newRecord = new WidRecoding(m_tab);
    m_wid_editRecord = new WidEdit(m_tab);
    connect(m_tab, SIGNAL(tabBarClicked(int)), this, SLOT(on_tabBarClicked(int)));
    connect(m_tab, SIGNAL(currentChanged(int)), this, SLOT(on_currentChanged(int)));

    m_tab->addTab(m_wid_dashboard, "DASHBOARD");
    m_tab->addTab(m_wid_livecall, "BROADCAST");
    m_tab->addTab(m_wid_newRecord, "NEW RECORDING");
    m_tab->addTab(m_wid_editRecord, "EDIT RECORDING");
    m_tab->setTabIcon(0, QPixmap(":/main/resource/dashboard.ico"));
    m_tab->setTabIcon(1, QPixmap(":/main/resource/livecall.ico"));
    m_tab->setTabIcon(2, QPixmap(":/main/resource/mic1.ico"));
    m_tab->setTabIcon(3, QPixmap(":/main/resource/edit.ico"));
    m_vLayout_main->addWidget(m_tab, 10);
    setLayout(m_vLayout_main);

    //it must be hidden right after initiation
    hide_main_window();

    qDebug() << "<< " << __FUNCTION__;
}


void MainWindow::show_main_window()
{
    m_lab_title->show();
    m_lab_user->show();
    m_bt_system->show();
    m_tab->show();
}
void MainWindow::hide_main_window()
{
    m_lab_title->hide();
    m_lab_user->hide();
    m_bt_system->hide();
    m_tab->hide();
}

void MainWindow::show_login_window()
{
    qDebug() << __FUNCTION__;
    m_loginWind->show();
}

void MainWindow::on_login_OK(const QString & user_name)
{
    qDebug()<< __FUNCTION__;
    m_user_name = user_name;
    m_lab_user->setText("Hello " + user_name);

    show_main_window();
    show();
}

void MainWindow::on_login_closed()
{
    qDebug() << __FUNCTION__;
}



void MainWindow::on_tabBarClicked(int index)
{
    qDebug() << "MainWindow::on_tabBarClicked, indx=" << index <<m_tab->tabText(index) ;

}
void MainWindow:: on_currentChanged(int index)
{
    qDebug() << "MainWindow::on_currentChanged, indx=" << index << m_tab->tabText(index);
}


void MainWindow::construct_system_menu()
{

    qDebug() << " >> " << __FUNCTION__;

    m_sys_menu = new QMenu(m_bt_system);
    QString style = "QMenu::item:selected { \
                         background-color:rgb(60, 60, 160);/*选中的样式*/\
                         }";
    m_sys_menu->setStyleSheet(style);

    QAction *action_logout = new QAction("Logout");
    QAction *action_pwd = new QAction("Change Password");
    QAction *action_settings = new QAction("Settings");
    QAction *action_logs = new QAction("Logs");
    QAction *action_exit = new QAction("Exit");
    m_sys_menu->addActions({action_logout,
                          action_pwd,
                          action_settings,
                          action_logs,
                          action_exit});
    m_sys_menu->insertSeparator(action_pwd);
    m_sys_menu->insertSeparator(action_exit);
    m_bt_system->setMenu(m_sys_menu);

    connect(action_logout, SIGNAL(triggered()), this, SLOT(on_menu_logout_clicked()));
    connect(action_pwd, SIGNAL(triggered()), this, SLOT(on_menu_pwd_clicked()));
    connect(action_settings, SIGNAL(triggered()), this, SLOT(on_menu_setting_clicked()));
    connect(action_logs, SIGNAL(triggered()), this, SLOT(on_menu_logs_clicked()));
    connect(action_exit, SIGNAL(triggered()), this, SLOT(on_menu_exit_clicked()));

    qDebug() << "<< " << __FUNCTION__;
}

void MainWindow::on_menu_logout_clicked()
{
    MyLog("Logout");
    hide_main_window();

    m_loginWind->clear_pwd();
    m_loginWind->show();

}
void MainWindow::on_menu_pwd_clicked()
{
    qDebug() << __func__;
    WidChangePwd *m_wid_chgpwd = new WidChangePwd(this);
    int ret = m_wid_chgpwd->exec();
    if(ret == QDialog::Accepted)
    {
        MyLog("login-password was changed successfully");
    }
    else
    {
        MyLog("login-password change was cancelled");
    }
    delete m_wid_chgpwd;
}
void MainWindow::on_menu_setting_clicked()
{}
void MainWindow::on_menu_logs_clicked()
{
    qDebug() << __func__;
    MyLog("Check Logs");
    widLogs *m_wid_logs = new widLogs(this);
    m_wid_logs ->exec();

    delete m_wid_logs;
}
void MainWindow::on_menu_exit_clicked()
{
    MyLog("Exit");
    //qApp->closeAllWindows();
    qApp->exit();
}

void MainWindow::closeEvent( QCloseEvent * event )
{
   int ret =  QMessageBox::information( this, tr("IHSS-UDE-800"),
                         tr("Do you really want to quit IHSS-UDE-800?"),
                         tr("Yes"), tr("No"),
                         0, 1 );
    switch(ret)
    {
       case 0:
            event->accept();
            MyLog("Exit by closing window");
           break;
       case 1:
    default:
           event->ignore();
           MyLog("Close-window is cancelled");
           break;
    }
}

void MainWindow::log(const QString &msg)
{
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    //QString current_date = QString("(%1)").arg(current_date_time);
    QString message = QString("[%1] %2: %3").arg(current_date_time, m_user_name, msg);

    QFile file("log.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.flush();
    file.close();
}




