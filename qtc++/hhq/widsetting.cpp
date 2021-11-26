#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include "widsetting.h"

WidSetting::WidSetting(QWidget *parent) : QWidget(parent)
{
    qDebug() << __FUNCTION__;

    //setting window
    //setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    setWindowTitle("Setting");
    setWindowIcon(QIcon(":/login/resource/setting.ico"));
    //no background color setting looks better.  87, 165, 186
    setStyleSheet(QString::fromUtf8("background-color: rgb(84, 207, 252);font:12pt 'Arial';\n")); //(84, 207, 252)rgb(43, 153, 240)
    resize(700, 500);

    QFont font = this->font();
    font.setFamily("Arial");
    font.setPointSize(12);
    font.setBold(false);
    setFont(font);
    QVBoxLayout *m_vLayout_main = new QVBoxLayout(this);
    m_tab = new QTabWidget(this);
    //set background and other style
    m_tab->setAttribute(Qt::WA_StyledBackground);
    m_tab->setStyleSheet("QTabBar::tab{background-color:rgb(100, 200, 220);color:rgb(60, 60, 160);font:11pt 'Arial'}\
                         QTabBar::tab{margin-right: 10px; margin-left: 0px; height: 35px; min-width: 100px}\
                         QTabWidget::tab-bar{border-width:0px;}\
                         QTabBar::tab::selected{background-color:rgb(60, 60, 160);color:white;font:12pt 'Arial'; height: 38px}");
                         //QTabWidget::pane{border-style: outset;}  //tab and form的边界线上置

    m_tab->setFont(font);
    m_tab->setDocumentMode(true);

    m_widLocal = new WidSetLocal(this);
    m_widRemote1 = new WidSetRemote(1, this);
    m_widRemote2 = new WidSetRemote(2, this);


    m_tab->addTab(m_widLocal, "Local");
    m_tab->addTab(m_widRemote1, "Remote-1");
    m_tab->addTab(m_widRemote2, "Remote-2");

    connect(m_tab, SIGNAL(tabBarClicked(int)), this, SLOT(on_tabBarClicked(int)));
    m_vLayout_main->addWidget(m_tab);
    //setLayout(m_vLayout_main);

}

void WidSetting::on_tabBarClicked(int index)
{
    qDebug() << __FUNCTION__;
    if(index == 0)
    {
        //resize(300, 200);

    }
    else
    {
        //resize(700, 500);
    }
}

void WidSetting::showEvent(QShowEvent *event)
{
    qDebug() << "WidSetting::" <<__FUNCTION__;

    m_widLocal->load_config();
    m_widRemote1->load_config();
    m_widRemote2->load_config();

}

WidSetting::~WidSetting()
{
    qDebug() << __FUNCTION__;
}
