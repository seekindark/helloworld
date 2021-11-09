#include <QWidget>
#include <QDebug>
#include <QDateTime>
#include <QFile>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTabWidget>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    // fix size, and disable maximize
    resize(800, 600);
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
    qDebug() << __FUNCTION__;

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
    font.setPointSize(20);
    m_lab_title->setFont(font);

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
    connect(m_bt_system, SIGNAL(clicked()), this, SLOT(on_system_clicked()));

    QVBoxLayout *m_vLayout_main = new QVBoxLayout(this);
    m_vLayout_main->setContentsMargins(50,50+20,50,0);
    m_tab = new QTabWidget(this);
    m_tab->setFont(font);
    m_tab->setDocumentMode(true);

    m_wid_dashboard = new QWidget(m_tab);
    m_wid_livecall = new QWidget(m_tab);
    m_wid_newRecord = new QWidget(m_tab);
    m_wid_editRecord = new QWidget(m_tab);
    m_wid_logs = new QWidget(m_tab);

    m_tab->addTab(m_wid_dashboard, "Dashboard");
    m_tab->addTab(m_wid_livecall, "LIVE CALL");
    m_tab->addTab(m_wid_newRecord, "NEW RECORDING");
    m_tab->addTab(m_wid_editRecord, "EDIT RECORDING");
    m_tab->addTab(m_wid_logs, "LOGS");
    m_vLayout_main->addWidget(m_tab, 10);
    setLayout(m_vLayout_main);

    //it must be hidden right after initiation
    hide_main_window();
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

void MainWindow::on_system_clicked()
{
    hide_main_window();
    m_loginWind->show();
}

void MainWindow::log(const QString &msg)
{
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    //QString current_date = QString("(%1)").arg(current_date_time);
    QString message = QString("[%1] %2: %3").arg(current_date_time).arg(m_user_name).arg(msg);

    QFile file("log.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.flush();
    file.close();
}




